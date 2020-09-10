# Imports

import tensorflow as tf
import workspace.Operations as op
from workspace.config import Config as config


# For imageSize = 56
def MobileNetImplementation(X, is_training):
    with tf.variable_scope('MN-Conv1'):
        X = op.Conv(X, nbFilters=32, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv1'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv1-Conv'):
            X = op.Conv(X, nbFilters=64*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv2'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv2-Conv'):
            X = op.Conv(X, nbFilters=128*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv3'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv3-Conv'):
            X = op.Conv(X, nbFilters=128*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv4'):
        X = op.DWConv(X, kernel=3, stride=2)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv4-Conv'):
            X = op.Conv(X, nbFilters=256*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv5'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv5-Conv'):
            X = op.Conv(X, nbFilters=256*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv6', reuse=tf.AUTO_REUSE):
        X = op.DWConv(X, kernel=3, stride=2)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv6-Conv'):
            X = op.Conv(X, nbFilters=512*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv7'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv7-Conv'):
            X = op.Conv(X, nbFilters=512*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv8'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv8-Conv'):
            X = op.Conv(X, nbFilters=512*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv9'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv9-Conv'):
            X = op.Conv(X, nbFilters=512*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv10'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv10-Conv'):
            X = op.Conv(X, nbFilters=512*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv11'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv11-Conv'):
            X = op.Conv(X, nbFilters=512*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv12'):
        X = op.DWConv(X, kernel=3, stride=2)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv12-Conv'):
            X = op.Conv(X, nbFilters=1024*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-DWConv13'):
        X = op.DWConv(X, kernel=3, stride=1)
        X = op.BatchNorm(X, is_training=is_training)
        X = op.ReLu(X)

        with tf.variable_scope('MN-DWConv13-Conv'):
            X = op.Conv(X, nbFilters=1024*config.complexity, kernel=1, stride=1)
            X = op.BatchNorm(X, is_training=is_training)
            X = op.ReLu(X)

    with tf.variable_scope('MN-FC', reuse=tf.AUTO_REUSE):
        X = tf.reduce_mean(X, axis=[1, 2], name='GlobalAVGPool')
        X = op.ConnectedLayer(X, config.num_class)

    return X