
# Imports

import tensorflow as tf
from workspace.config import Config as config

"""
    Different functions for the ShuffleNet implementation
"""

# Functions

def Conv(X, nbFilters, kernel, stride):
    """
        :param X: input tensor
        :param nbFilters: number of filters
        :param kernel: kernel shape for convolution operation
        :param stride: stride for convolution operation
        :return: Convolution operation with bias
    """

    channels = X.shape.as_list()[-1]

    F = tf.get_variable('kernel', [kernel, kernel, channels, nbFilters],
                        tf.float32, tf.contrib.layers.xavier_initializer())

    b = tf.get_variable('Cbias', [nbFilters], tf.float32, tf.zeros_initializer())

    return tf.nn.bias_add(tf.nn.conv2d(X, F, [1, stride, stride, 1], 'SAME'), b)


def GConv(X, nbFilters, groups, kernel, stride):
    """
        :param X: input tensor
        :param nbFilters: number of filters
        :param groups: number of groups
        :param kernel: kernel shape for group convolution operation
        :param stride: stride for group convolution operation
        :return: group convolution operation
    """
    channels = X.shape.as_list()[-1]
    channelsPerGroup = int(channels / groups)
    nbFiltersPerGroup = int(nbFilters / groups)

    F = tf.get_variable('kernel', [kernel, kernel, channelsPerGroup, nbFilters],
                        tf.float32, tf.contrib.layers.xavier_initializer())

    # split channels
    X_channel_splits = tf.split(X, [channelsPerGroup] * groups, axis=3)
    F_filter_splits = tf.split(F, [nbFiltersPerGroup] * groups, axis=3)

    results = []

    # do convolution for each split
    for i in range(groups):
        X_split = X_channel_splits[i]
        F_split = F_filter_splits[i]
        results += [tf.nn.conv2d(X_split, F_split, [1, stride, stride, 1], 'SAME')]

    return tf.concat(results, 3)


def DWConv(X, kernel, stride):
    """
        :param X: input tensor
        :param kernel: kernel shape for depthwise convolution operation
        :param stride: stride for depthwise convolution operation
        :return: depthwise convolution operation
    """
    channels = X.shape.as_list()[-1]

    DW = tf.get_variable('kernel', [kernel, kernel, channels, 1], tf.float32,
                         tf.contrib.layers.xavier_initializer())

    return tf.nn.depthwise_conv2d(X, DW, [1, stride, stride, 1], 'SAME')


def ConnectedLayer(X, size):
    """
        :param X: input tensor
        :param size: final size
        :return: fully connected layer
    """
    inputSize = X.shape.as_list()[1]

    W = tf.get_variable('kernel', [inputSize, size], tf.float32, tf.contrib.layers.xavier_initializer())

    b = tf.get_variable('FCBias', [size], tf.float32, tf.zeros_initializer())

    return tf.nn.bias_add(tf.matmul(X, W), b)



def Shuffle(X, groups):
    """
        :param X: input tensor
        :param groups: number of groups
        :return: shuffle operation
    """
    height, width, channels = X.shape.as_list()[1:]
    channelsPerGroup = int(channels / groups)

    # reshape
    shape = tf.stack([-1, height, width, groups, channelsPerGroup])
    X = tf.reshape(X, shape)

    # transpose
    X = tf.transpose(X, [0, 1, 2, 4, 3])

    # reshape
    shape = tf.stack([-1, height, width, channels])
    X = tf.reshape(X, shape)

    return X


def AVGPool(X, ksize, stride):
    """
        :param X: input tensor
        :param ksize: kernel shape for average pooling operation
        :param stride: stride for average pooling operation
        :return: average pooling operation
    """
    return tf.nn.avg_pool(X, [1, ksize, ksize, 1], [1, stride, stride, 1], 'SAME', name='AVGPool')


def MaxPool(X, ksize, stride):
    """
        :param X: input tensor
        :param ksize: kernel shape for max pooling operation
        :param stride: stride for max pooling operation
        :return: max pooling operation
        """
    return tf.nn.max_pool(X, [1, ksize, ksize, 1], [1, stride, stride, 1], 'SAME', name='MaxPool')


def BatchNorm(X, is_training):
    """
        :param X: input tensor
        :return: batch normalization operation
    """
    return tf.layers.batch_normalization(X, scale=False, center=True, momentum=config.batchMomentum,
                                         training=is_training, fused=True, name='BatchNorm')


def ReLu(X):
    """
        :param X: input tensor
        :return: ReLu operation
    """
    return tf.nn.relu(X, name='ReLu')

def Regularization():
    """
        :param reg: regularization term
        :return: Add regularization
    """

    reg = tf.constant(config.reg, tf.float32, [], 'weight_decay')

    trainable = tf.get_collection(tf.GraphKeys.TRAINABLE_VARIABLES)
    kernels = [v for v in trainable if 'kernel' in v.name]

    for K in kernels:
        l2_loss = tf.multiply(reg, tf.nn.l2_loss(K))
        tf.losses.add_loss(l2_loss)


def ReduceLR(sess, ops, valAcc):
    """
        :param sess: Session of the the graph
        :param ops: Operation to apply
        :param valAcc: Validation accuracy list
    :return:
    """
    if len(valAcc) > 10:
        average = (valAcc[-10] + valAcc[-9] + valAcc[-8] + valAcc[-7] + valAcc[-6]) / 5.0
        if valAcc[-1] < (average + 0.01):
            sess.run(ops['drop_eta'])




