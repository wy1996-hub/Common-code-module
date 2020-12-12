import paddle
import paddle.fluild as fuild
import numpy as np
from paddle.fluid.dygraph.nn import Conv2D, Pool2D, Linear

# 定义 AlexNet 网络结构
class AlexNet(paddle.fluid.dygraph):
    def __init__(self, num_classes=1000):
        super(AlexNet, self).__init__()

        # AlexNet与LeNet一样使用卷积层与池化层提取图像特征
        # 不同的是AlexNet激活函数换成了relu
        self.conv1 = Conv2D(num_channels=3, num_filters=96, filter_size=11, stride=4, padding=5, act='relu')
        # 对于每一个卷积层，激活操作是包含在卷积层中的
        self.pool1 = Pool2D(pool_size=2, pool_type='max', pool_stride=2)
        self.conv2 = Conv2D(num_channels=96, num_filters=256, filter_size=5, stride=1, padding=2, act='relu')
        self.pool2 = Pool2D(pool_size=2, pool_type='max', pool_stride=2)
        self.conv3 = Conv2D(num_channels=256, num_filters=384, filter_size=3, stride=1, padding=1, act='relu')
        self.conv4 = Conv2D(num_channels=384, num_filters=384, filter_size=3, stride=1, padding=1, act='relu')
        self.conv5 = Conv2D(num_channels=384, num_filters=256, filter_size=3, stride=1, padding=1, act='relu')
        self.pool5 = Pool2D(pool_size=2, pool_type='max', pool_stride=2)
        self.fc1 = Linear(input_dim=12544, output_dim=4096, act='relu')
        self.drop_ratio1 = 0.5
        # AlexNet的一个改进就是引入了dropout
        # 在全连接之后使用dropout抑制过拟合
        self.fc2 = Linear(input_dim=4096, output_dim=4096, act='relu')
        self.drop_ratio2 = 0.5
        self.fc3 = Linear(input_dim=4096, output_dim=num_classes, act='softmax')

        # 卷积核的厚度 = 被卷积的图像的通道数
        # 卷积核的个数 = 卷积操作后输出的通道数
        # [N-C-H-W] = [卷积核个数-卷积核通道数（厚度）- 卷积核高度-卷积核宽度]
        # 注意，卷积输出层的[N-C-H-W]与卷积核的[N-C-H-W]不是完全一样的

    def forward(self, x):
        x = self.conv1(x)
        x = self.pool1(x)
        x = self.conv2(x)
        x = self.pool2(x)
        x = self.conv3(x)
        x = self.conv4(x)
        x = self.conv5(x)
        x = self.pool5(x)
        x = fluid.layers.reshape(x, [x.shape[0], -1])
        x = self.fc1(x)
        x = fluid.layers.dropout(x, self.drop_ratio1)
        x = self.fc2(x)
        x = fluid.layers.dropout(x, self.drop_ratio2)
        x = self.fc3(x)

