# Imports

import os

class Config(object):
  """
    Model configuration
  """

  version = 'v1' # 'v1' or 'v2'
  model = 'shufflenet' # 'shufflenet' or 'mobilenet'

  size_train = 100000
  size_val = 10000
  num_class = 200

  sizeImage = 56

  batch_size = 200
  num_epochs = 50

  trainSteps = int(size_train/batch_size)
  valSteps = int(size_val/batch_size)

  eta = 0.1  # learning rate
  drop = 0.1 # decrease learning rate
  reg = 0.005 # regularization term
  batchMomentum = 0.1
  momentum = 0.9

  complexity = 0.25 # 0.25 or 0.5 or 1

  groups = 1 # 1, 2, 3, 4, 8

  if version == 'v1':
    filters = [144, 200, 240, 272, 384]
  elif version == 'v2':
    filters = 116

  if sizeImage == 56:
    repeat = [1, 3, 1]
    strideFirstLayer = 1
  elif sizeImage == 224:
    repeat = [3, 7, 3]
    strideFirstLayer = 2

  if complexity == 0.25:
        resultPath = os.path.join(os.getcwd(), "results",
                                  'S' + str(sizeImage) + '_E' + str(num_epochs) + '_G' + str(groups) + '_C025')
  elif complexity == 0.5:
        resultPath = os.path.join(os.getcwd(), "results",
                                  'S' + str(sizeImage) + '_E' + str(num_epochs) + '_G' + str(groups) + '_C050')
  elif complexity == 1:
        resultPath = os.path.join(os.getcwd(), "results",
                                  'S' + str(sizeImage) + '_E' + str(num_epochs) + '_G' + str(groups) + '_C100')

  if model == 'mobilenet':
    resultPath = os.path.join(resultPath, '_MobileNet')

  if version == 'v2':
    resultPath = os.path.join(resultPath, '_v2')

  if not os.path.exists(resultPath):
      os.makedirs(resultPath)
