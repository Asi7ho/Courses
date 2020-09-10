
# Imports

import workspace.LoadData as ld
import workspace.Model as model
from workspace.config import Config as config
import matplotlib.pyplot as plt
import tensorflow as tf
import os

# Disable Tensorflow Debugging
tf.logging.set_verbosity(tf.logging.ERROR)


def main():

    # Dataset path
    datasetPath = os.path.join(os.getcwd(), '04_datasets', 'tiny-imagenet-200')

    # Folder for tfRecords
    TfRecordPath = os.path.join(datasetPath, 'TfRecords')

    if not os.path.exists(TfRecordPath):
        os.makedirs(TfRecordPath)

    # Files
    trainFile = os.path.join(TfRecordPath, 'training.tfrecords')
    valFile = os.path.join(TfRecordPath, 'validation.tfrecords')

    # Convert to tfRecords
    convertToTfRecord = False

    if convertToTfRecord:
        print('############# Loading TfRecords Datasets #################')
        print('------> Convert to TfRecords')
        ld.convertToTfRecord(datasetPath, 'train', trainFile)
        print('--> Training set converted')
        ld.convertToTfRecord(datasetPath, 'val', valFile)
        print('--> Validation set converted')
        print('------> End of conversion')

    print('############# TfRecords Datasets Loaded #################')

    # Training process
    print('############# Training Process #################')
    graph, trainDataLoss, trainDataAcc, valDataLoss, valDataAcc = model.train(trainFile, valFile)

    # Plot performance

    t = [i for i in range(1, config.num_epochs+1)]

    fig1 = plt.figure(1, figsize=(15, 10))
    ax1 = fig1.add_subplot(1, 2, 1)
    ax1.plot(t, trainDataLoss, label='Training Loss')
    ax1.plot(t, valDataLoss, label='Validation Loss')
    ax1.set_xlabel('Epochs')
    ax1.set_ylabel('Loss')
    ax1.set_title('Loss')
    ax1.legend()

    ax2 = fig1.add_subplot(1, 2, 2)
    ax2.plot(t, trainDataAcc, label='Training Accuracy')
    ax2.plot(t, valDataAcc, label='Validation Accuracy')
    ax2.set_xlabel('Epochs')
    ax2.set_ylabel('Accuracy')
    ax2.set_title('Accuracy')
    ax2.legend()
    fig1.savefig(os.path.join(config.resultPath, "Performance.png"))

    # FLOPs computation | Put batch_size = 1
    computeFLOPs = True

    if computeFLOPs:
        print('############# FLOPs Computation #################')
        flopPath = os.path.join(os.getcwd(), "results", "FLOPs")
        if not os.path.exists(flopPath):
            os.makedirs(flopPath)

        flops = tf.profiler.profile(graph, options=tf.profiler.ProfileOptionBuilder.float_operation(), cmd='scope')

        with open(os.path.join(flopPath, 'FLOPs.txt'), 'a+') as f:
            f.write('Model needs {} FLOPS | Config : S{}, G{}, C{}\n'.
                    format(flops.total_float_ops, config.sizeImage, config.groups, config.complexity))

        print('Model needs {} FLOPS | Config : S{}, G{}, C{}'.
              format(flops.total_float_ops, config.sizeImage, config.groups, config.complexity))

    return


if __name__ == '__main__':
    main()
