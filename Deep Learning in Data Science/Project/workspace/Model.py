
# Imports

import tensorflow as tf
from tqdm import tqdm
import workspace.Operations as op
import workspace.LoadData as ld
import workspace.MobileNet as MobileNet
from workspace.config import Config as config
import os

def summarise():
    """Add histograms of all trainable variables."""

    summaries = []
    trainable_vars = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES)

    for v in trainable_vars:
        summaries += [tf.summary.histogram(v.name[:-2] + '_hist', v)]

    return tf.summary.merge(summaries)


def shuffleUnit(X, nbFilter, groups, firstLayer, stride, is_training):
    """
        :param X: input tensor
        :param nbFilter: number of filters
        :param groups: number of groups
        :param firstLayer: Boolean for the first layer of Stage 2
        :param stride: stride for the different operations
        :return: shuffleNet unit operation
    """
    if config.version == 'v1':
        if firstLayer:
            R = X
            channel = X.shape.as_list()[-1]
            nbFilter -= channel
            with tf.variable_scope('FL1-GConv1'):
                R = op.GConv(R, nbFilters=nbFilter, groups=1, kernel=1, stride=1)
                R = op.BatchNorm(R, is_training=is_training)
                R = op.ReLu(R)

            with tf.variable_scope('FL1-DWConv'):
                R = op.DWConv(R, kernel=3, stride=2)
                R = op.BatchNorm(R, is_training=is_training)

            with tf.variable_scope('FL1-GConv2'):
                R = op.GConv(R, nbFilters=nbFilter, groups=groups, kernel=1, stride=1)
                R = op.BatchNorm(R, is_training=is_training)

            X = op.AVGPool(X, ksize=3, stride=2)
            R = tf.concat([R, X], 3)
            R = op.ReLu(R)
            return R

        else:
            R = X
            channel = X.shape.as_list()[-1]
            with tf.variable_scope('SU-GConv1'):
                R = op.GConv(R, nbFilters=channel, groups=groups, kernel=1, stride=1)
                R = op.BatchNorm(R, is_training=is_training)
                R = op.ReLu(R)

            with tf.variable_scope('SU-Shuffle'):
                R = op.Shuffle(R, groups=groups)

            with tf.variable_scope('SU-DWConv'):
                R = op.DWConv(R, kernel=3, stride=stride)
                R = op.BatchNorm(R, is_training=is_training)

            with tf.variable_scope('SU-GConv2'):
                R = op.GConv(R, nbFilters=channel, groups=groups, kernel=1, stride=1)
                R = op.BatchNorm(R, is_training=is_training)

            if stride < 2:
                R = tf.add(R, X)
            else:
                X = op.AVGPool(X, ksize=3, stride=2)
                R = tf.concat([R, X], 3)

            R = op.ReLu(R)
            return R

    elif config.version == 'v2':
        if stride == 1:
            channel = X.shape.as_list()[-1]
            R1, R2 = tf.split(X, num_or_size_splits=2, axis=3)

            with tf.variable_scope('SU-Conv1'):
                R2 = op.Conv(R2, nbFilters=channel, kernel=1, stride=1)
                R2 = op.BatchNorm(R2, is_training=is_training)
                R2 = op.ReLu(R2)

            with tf.variable_scope('SU-DWConv'):
                R2 = op.DWConv(R2, kernel=3, stride=stride)
                R2 = op.BatchNorm(R2, is_training=is_training)

            with tf.variable_scope('SU-Conv2'):
                R2 = op.Conv(R2, nbFilters=channel, kernel=1, stride=1)
                R2 = op.BatchNorm(R2, is_training=is_training)
                R2 = op.ReLu(R2)

            R = tf.concat([R1, R2], 3)
            R = op.Shuffle(R, groups=1)
            return R

        elif stride == 2:
            channel = X.shape.as_list()[-1]
            R = X

            with tf.variable_scope('SU-Conv1'):
                R = op.Conv(R, nbFilters=channel, kernel=3, stride=1)
                R = op.BatchNorm(R, is_training=is_training)
                R = op.ReLu(R)

            with tf.variable_scope('SU-DWConv1'):
                R = op.DWConv(R, kernel=1, stride=stride)
                R = op.BatchNorm(R, is_training=is_training)

            with tf.variable_scope('SU-Conv2'):
                R = op.Conv(R, nbFilters=channel, kernel=1, stride=1)
                R = op.BatchNorm(R, is_training=is_training)
                R = op.ReLu(R)

            with tf.variable_scope('SU-DWConv2'):
                X = op.DWConv(X, kernel=3, stride=stride)
                X = op.BatchNorm(X, is_training=is_training)

            with tf.variable_scope('SU-Conv3'):
                X = op.Conv(X, nbFilters=channel, kernel=1, stride=1)
                X = op.BatchNorm(X, is_training=is_training)
                X = op.ReLu(X)

            R = tf.concat([R, X], 3)
            R = op.Shuffle(R, groups=1)
            return R

def shuffleNetImplementation(X, is_training):
    """
        :param X: input tensor
        :return: shuffleNet implementation with the different stages
    """

    if config.version == 'v1':
        if config.groups == 1:
            out_channels = config.filters[0]
        elif config.groups == 2:
            out_channels = config.filters[1]
        elif config.groups == 3:
            out_channels = config.filters[2]
        elif config.groups == 4:
            out_channels = config.filters[3]
        elif config.groups == 8:
            out_channels = config.filters[4]

        out_channels = out_channels * config.complexity

    elif config.version == 'v2':
        if config.complexity == 0.5:
            out_channels = 48
        elif config.complexity == 1:
            out_channels = 116


    # Stage 1
    with tf.variable_scope('Stage1'):
        with tf.variable_scope('Conv1'):
            X = op.Conv(X, nbFilters=24, kernel=3, stride=config.strideFirstLayer)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)
        with tf.variable_scope('MaxPool'):
            X = op.MaxPool(X, ksize=3, stride=2)

    # Stage 2
    with tf.variable_scope('Stage2'):
        with tf.variable_scope('S1'):
            X = shuffleUnit(X, stride=2, nbFilter=out_channels, groups=config.groups, firstLayer=True, is_training=is_training)
        for i in range(config.repeat[0]):
            with tf.variable_scope('S' + str(i + 2)):
                X = shuffleUnit(X, stride=1, nbFilter=0, groups=config.groups, firstLayer=False, is_training=is_training)

    # Stage 3
    with tf.variable_scope('Stage3'):
        with tf.variable_scope('S1'):
            X = shuffleUnit(X, stride=2, nbFilter=0, groups=config.groups, firstLayer=False, is_training=is_training)
        for i in range(config.repeat[1]):
            with tf.variable_scope('S' + str(i + 2)):
                X = shuffleUnit(X, stride=1, nbFilter=0, groups=config.groups, firstLayer=False, is_training=is_training)

    # Stage 4
    with tf.variable_scope('Stage4'):
        with tf.variable_scope('S1'):
            X = shuffleUnit(X, stride=2, nbFilter=0, groups=config.groups, firstLayer=False, is_training=is_training)
        for i in range(config.repeat[2]):
            with tf.variable_scope('S' + str(i + 2)):
                X = shuffleUnit(X, stride=1, nbFilter=0, groups=config.groups, firstLayer=False, is_training=is_training)

    # Stage 5 | v2
    if config.version == 'v2':
        with tf.variable_scope('Stage5'):
            with tf.variable_scope('Conv5'):
                X = op.Conv(X, nbFilters=1024, kernel=1, stride=1)
                X = op.BatchNorm(X, is_training=is_training)
                X = op.ReLu(X)

    # End on the implementation
    with tf.variable_scope('FC'):
        X = tf.reduce_mean(X, axis=[1, 2], name='GlobalAVGPool')
        X = op.ConnectedLayer(X, config.num_class)

    return X


def NetGraph():
    """
        :return: graph of the shuffleNet
    """
    graph = tf.Graph()

    with graph.as_default():
        with tf.variable_scope('control'):
            is_training = tf.placeholder_with_default(True, [], 'is_training')

        with tf.variable_scope('inputPipeline'):
            data = ld.GetData()

        with tf.variable_scope('inputs'):
            X = tf.placeholder_with_default(data['X_batch'], [None, config.sizeImage, config.sizeImage, 3], 'X')
            Y = tf.placeholder_with_default(data['y_batch'], [None, config.num_class], 'Y')

        if config.model == 'shufflenet':
            logits = shuffleNetImplementation(X, is_training=is_training)
        elif config.model == 'mobilenet':
            logits = MobileNet.MobileNetImplementation(X, is_training=is_training)

        # SGD + Momentum + Nesterov optimizer http://cs231n.github.io/neural-networks-3/#ada
        with tf.variable_scope('softmax'):
            predictions = tf.nn.softmax(logits, name='output')

        with tf.variable_scope('loss'):
            loss = tf.losses.softmax_cross_entropy(Y, logits)

        with tf.variable_scope('regularization'):
            op.Regularization()

        with tf.variable_scope('total_loss'):
            total_loss = tf.losses.get_total_loss()

        with tf.variable_scope('eta'):
            eta = tf.Variable(config.eta, trainable=False, dtype=tf.float32, name='eta')
            drop_eta = tf.assign(eta, config.drop * eta)

        update_ops = tf.get_collection(tf.GraphKeys.UPDATE_OPS)
        with tf.control_dependencies(update_ops), tf.variable_scope('optimizer'):
            optimizer = tf.train.MomentumOptimizer(eta, momentum=config.momentum, use_nesterov=True)
            grads = optimizer.compute_gradients(total_loss)
            optimize = optimizer.apply_gradients(grads)

        grad_summaries = tf.summary.merge([tf.summary.histogram(v.name[:-2] + '_grad_hist', g) for g, v in grads])

        with tf.variable_scope('utilities'):
            initVariables = tf.global_variables_initializer()
            is_equal = tf.equal(tf.argmax(predictions, 1), tf.argmax(Y, 1))
            accuracy = tf.reduce_mean(tf.cast(is_equal, tf.float32))

        summaries = summarise()

    graph.finalize()

    ops = {
        # initialization
        'init_variables': initVariables,
        'init_data': data['init_data'],
        'train_init': data['train_init'],
        'val_init': data['val_init'],

        # training
        'optimize': optimize, 'drop_eta': drop_eta,

        # evaluation
        'predictions': predictions,
        'loss': total_loss, 'accuracy': accuracy,

        # summary
        'summaries': summaries, 'grad_summaries': grad_summaries,
    }

    return graph, ops


def train(trainFile, valFile):
    """
        :param trainFile: train tfRecords
        :param valFile: val tfRecords
        :return: training process
    """

    graph, ops = NetGraph()

    configuration = tf.ConfigProto(allow_soft_placement=True)
    configuration.gpu_options.allow_growth = True

    sess = tf.Session(graph=graph, config=configuration)
    writer = tf.summary.FileWriter(config.resultPath, sess.graph)

    # Initialize the vairiables
    sess.run(ops['init_variables'])

    # Initialize data
    data_dict = {
        'inputPipeline/train_file:0': trainFile,
        'inputPipeline/val_file:0': valFile,
        'inputPipeline/batch_size:0': config.batch_size
    }
    sess.run(ops['init_data'], data_dict)

    trainDataLoss = []
    trainDataAcc = []
    valDataLoss = []
    valDataAcc = []

    # Loop
    for epoch in range(config.num_epochs):

        # Training dataset
        trainLoss, trainAcc = 0.0, 0.0

        sess.run(ops['train_init'])

        run_options = tf.RunOptions(trace_level=tf.RunOptions.FULL_TRACE)
        run_metadata = tf.RunMetadata()

        _, batch_loss, batch_accuracy, summary, grad_summary = sess.run([ops['optimize'], ops['loss'], ops['accuracy'],
                                                                         ops['summaries'], ops['grad_summaries']],
                                                                        options=run_options, run_metadata=run_metadata)

        trainLoss += batch_loss
        trainAcc += batch_accuracy

        print('################# epoch', epoch+1, ' ################# ')
        training_steps = tqdm(range(1, config.trainSteps), initial=1, total=config.trainSteps)

        for _ in training_steps:
            _, batch_loss, batch_accuracy = sess.run([ops['optimize'], ops['loss'], ops['accuracy']])

            trainLoss += batch_loss
            trainAcc += batch_accuracy

        # Validation dataset
        valLoss, valAcc = 0.0, 0.0

        sess.run(ops['val_init'])

        for _ in range(config.valSteps):
            batch_loss, batch_accuracy = sess.run([ops['loss'], ops['accuracy']], {'control/is_training:0': False})
            valLoss += batch_loss
            valAcc += batch_accuracy

        trainLoss /= config.trainSteps
        trainAcc /= config.trainSteps
        valLoss /= config.valSteps
        valAcc /= config.valSteps

        trainDataLoss.append(trainLoss)
        trainDataAcc.append(trainAcc)
        valDataLoss.append(valLoss)
        valDataAcc.append(valAcc)

        writer.add_run_metadata(run_metadata, str(epoch+1))
        writer.add_summary(summary, epoch+1)
        writer.add_summary(grad_summary, epoch+1)

        op.ReduceLR(sess, ops, valDataAcc)

        with open(os.path.join(config.resultPath, 'result.txt'), 'a+') as f:
            f.write('Epoch: {} (Complexity : {})\n'.format(epoch+1, config.complexity))
            f.write('Train loss = {0: .3f}, Train acc = {1: .3f},\t'.format(trainDataLoss[-1], trainDataAcc[-1]))
            f.write('Val loss = {0: .3f}, Val acc = {1: .3f}\n\n'.format(valDataLoss[-1], valDataAcc[-1]))

        print('Train loss = {0: .3f}, Train acc = {1: .3f}'.format(trainDataLoss[-1], trainDataAcc[-1]))
        print('Val loss = {0: .3f}, Val acc = {1: .3f}\n'.format(valDataLoss[-1], valDataAcc[-1]))

    sess.close()

    return graph, trainDataLoss, trainDataAcc, valDataLoss, valDataAcc
