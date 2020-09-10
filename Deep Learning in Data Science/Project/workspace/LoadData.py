
# Imports

import glob
import re
import tensorflow as tf
import numpy as np
from workspace.config import Config as config
import os


def buildDict(dataset):
    """
        :param dataset: path of the dataset
        :return: dictionary label-description
    """
    labelDict = {}
    classDescription = {}

    with open(os.path.join(dataset, 'wnids.txt'), 'r') as f:
        for i, line in enumerate(f.readlines()):
            files = line[:-1]
            labelDict[files] = i

    with open(os.path.join(dataset, 'words.txt'), 'r') as f:
        for i, line in enumerate(f.readlines()):
            file, desc = line.split("\t")
            desc = desc[:-1]

            if file in labelDict:
                classDescription[labelDict[file]] = desc

    return labelDict, classDescription


def loadFilenameAndLabels(dataset, mode):
    """
        :param dataset: path of the dataset
        :param mode: train or val
        :return: lists of images path and the labels
    """
    labelDict, classDescription = buildDict(dataset)
    imagesPathList = []
    labelList = []

    if mode == 'train':
        filenames = glob.glob(os.path.join(dataset, 'train', '*', 'images', '*.JPEG'))
        for file in filenames:
            match = re.search(r'n\d+', file)
            label = str(labelDict[match.group()])
            imagesPathList.append(file)
            labelList.append(int(label))

    elif mode == 'val':
        with open(os.path.join(dataset, 'val', 'val_annotations.txt'), 'r') as f:
            for line in f.readlines():
                splitLine = line.split('\t')
                fileName = os.path.join(dataset, 'val', 'images', splitLine[0])
                label = labelDict[splitLine[1]]
                imagesPathList.append(fileName)
                labelList.append(int(label))

    return imagesPathList, labelList


def encodeImage(imagePath, label):
    """
        :param imagePath: path of an image
        :param label: label of the image
        :return: example for tfRecords of the image
    """

    # Use byte encoding to reduce the size of the tfRecords
    with tf.gfile.FastGFile(imagePath, 'rb') as fid:
        image_data = fid.read()

    features = {
        'image': tf.train.Feature(bytes_list=tf.train.BytesList(value=[image_data])),
        'label': tf.train.Feature(int64_list=tf.train.Int64List(value=[label]))
        }

    example = tf.train.Example(features=tf.train.Features(feature=features))

    return example

def convertToTfRecord(datasetPath, mode, tfrecord_file_name):
    """
        :param datasetPath: path of the dataset
        :param mode: train or val
        :param tfrecord_file_name: name of the tfRecords
        :return: tfRecords of the dataset
    """
    imagePath, labelList = loadFilenameAndLabels(datasetPath, mode)

    with tf.python_io.TFRecordWriter(tfrecord_file_name) as writer:
        for i, imgp in enumerate(imagePath):
            example = encodeImage(imgp, labelList[i])
            writer.write(example.SerializeToString())


def readImageTrain(tfRecordsName):
    """
        :param tfRecordsName: tfRecords name
        :return: map function for the training dataset
    """
    features = {
        'image': tf.FixedLenFeature([], tf.string),
        'label': tf.FixedLenFeature([], tf.int64)
    }

    sample = tf.parse_single_example(tfRecordsName, features)
    label = sample['label']

    img = tf.image.decode_jpeg(sample['image'], channels=3)
    img = tf.image.convert_image_dtype(img, tf.float32)

    img = tf.random_crop(img, np.array([56, 56, 3]))
    img = tf.image.resize(img, [config.sizeImage, config.sizeImage])
    img = tf.image.random_flip_left_right(img)
    img = tf.image.random_brightness(img, 0.15)
    img = tf.image.random_contrast(img, 0.8, 1.25)
    img = tf.image.random_hue(img, 0.1)
    img = tf.image.random_saturation(img, 0.8, 1.25)
    return img, label


def readImageVal(tfRecordsName):
    """
        :param tfRecordsName: tfRecords name
        :return: map function for the validation dataset
    """
    features = {
        'image': tf.FixedLenFeature([], tf.string),
        'label': tf.FixedLenFeature([], tf.int64)
    }

    sample = tf.parse_single_example(tfRecordsName, features)
    label = sample['label']

    img = tf.image.decode_jpeg(sample['image'], channels=3)
    img = tf.image.convert_image_dtype(img, tf.float32)
    img = tf.image.crop_to_bounding_box(img, 4, 4, 56, 56)
    img = tf.image.resize(img, [config.sizeImage, config.sizeImage])
    return img, label


def GetData():
    """
        :return: initialization of the data
    """
    batchSize = tf.Variable(tf.placeholder(tf.int64, [], 'batch_size'), trainable=False, collections=[])
    trainFile = tf.Variable(tf.placeholder(tf.string, [], 'train_file'), trainable=False, collections=[])
    valFile = tf.Variable(tf.placeholder(tf.string, [], 'val_file'), trainable=False, collections=[])

    initData = tf.variables_initializer([batchSize, trainFile, valFile])

    trainDataset = tf.data.TFRecordDataset(trainFile)
    trainDataset = trainDataset.map(readImageTrain, num_parallel_calls=tf.data.experimental.AUTOTUNE).prefetch(10*config.batch_size)
    trainDataset = trainDataset.shuffle(buffer_size=100*config.batch_size)
    trainDataset = trainDataset.batch(batchSize)

    valDataset = tf.data.TFRecordDataset(valFile)
    valDataset = valDataset.map(readImageVal)
    valDataset = valDataset.batch(batchSize)

    iterator = tf.data.Iterator.from_structure(trainDataset.output_types, trainDataset.output_shapes)

    trainInit = iterator.make_initializer(trainDataset)
    valInit = iterator.make_initializer(valDataset)

    X_batch, y_batch = iterator.get_next()
    y_batch = tf.one_hot(y_batch, config.num_class, axis=1, dtype=tf.float32)

    data = {
        'init_data': initData,
        'train_init': trainInit, 'val_init': valInit,
        'X_batch': X_batch, 'y_batch': y_batch
    }

    return data

