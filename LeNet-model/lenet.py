import paddle
import paddle.fluid as fluid
import numpy as np
from paddle.fluid.dygraph.nn import Conv2D, Pool2D, Linear

# 定义 LeNet 网络结构
class LeNet(fluid.dygraph.Layer):
    def __init__(self, num_class=10):
        super(LeNet, self).__init__()

        # 创建卷积和池化层块，每个卷积层sigmoid激活函数，后面加一个2x2的池化
        self.conv1 = Conv2D(num_channels=1, num_filters=6, filter_size=5,  act='sigmoid')
        self.pool1 = Pool2D(pool_size=2, pool_type='max', pool_stride=2, pool_padding=0)
        self.conv2 = Conv2D(num_channels=6, num_filters=16, filter_size=5,  act='sigmoid')
        self.pool2 = Pool2D(pool_size=2, pool_type='max', pool_stride=2, pool_padding=0)
        # 创建第3个卷积层，此卷积层不含有池化层
        self.conv3 = Conv2D(num_channels=16, num_filters=120, filter_size=5, act='sigmoid')
        # 创建全连接层，卷积层的输出数据格式是[N,C,H,W]，在输入全连接层的时候，会自动将数据拉平
        # 也就是对每个样本，自动将其转化为长度为K的向量，K=C×H×W ，一个mini-batch的数据维度变成了N×K的二维向量
        self.fc1 = Linear(input_dim=120, output_dim=64, act='sigmoid')
        self.fc2 = Linear(input_dim=64, output_dim=num_classes, act='softmax')

    def forward(self, x):
        x = self.conv1(x)
        x = self.pool1(x)
        x = self.conv2(x)
        x = self.pool2(x)
        x = self.conv3(x)
        x = fluid.layers.reshape(x, [x.shape[0], -1])
        x = self.fc1(x)
        x = self.fc2(x)