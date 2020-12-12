import paddle
import paddle.fluid as fluid
from paddle.fluid.dygraph.nn import Conv2D, Pool2D, BatchNorm, Linear

# Google的研究人员为了向LeNet致敬，特地将模型命名为GoogLeNet
# 主要特点是网络不仅有深度，还在横向上具有“宽度”。
# 空间分布范围更广的图像信息适合用较大的卷积核来提取其特征；
# 而空间分布范围较小的图像信息则适合用较小的卷积核来提取其特征。
# 为了解决这个问题，GoogLeNet提出了一种被称为Inception模块的方案
# ----Inception一词来源于电影《盗梦空间》

# Inception模块的设计思想:使用3个不同大小的卷积核对输入图片进行卷积操作，并附加最大池化，将4个操作的输出沿着通道这一维度进行拼接
# 构成的输出特征图将会包含经过不同大小的卷积核提取出来的特征
# 最终输出特征图的通道数是每个支路输出通道数的总和
# 使用多个Inception模块串联操作的时候，模型参数量会变得非常大
# 为减小参数量，Inception在每个3x3和5x5的卷积层之前增加1x1的卷积层来控制输出通道数；在最大池化层后面增加1x1卷积层减小输出通道数

class Inception(fluid.dygraph.Layer):
    def __init__(self, c0, c1, c2, c3, c4):
        '''
        :param c1:第一条支路1x1卷积的输出通道数，数据类型是整数
        :param c2:第二条支路卷积的输出通道数，数据类型是tuple或list,其中c2[0]是1x1卷积的输出通道数，c2[1]是3x3
        :param c3:第三条支路卷积的输出通道数，数据类型是tuple或list,其中c3[0]是1x1卷积的输出通道数，c3[1]是3x3
        :param c4:第四条支路1x1卷积的输出通道数，数据类型是整数
        '''
        super(Inception, self).__init__()
        # 依次创建Inception块每条支路上使用到的操作
        self.p1_1 = Conv2D(num_channels=c0, num_filters=c1, filter_size=1, act='relu')
        self.p2_1 = Conv2D(num_channels=c0, num_filters=c2[0], filter_size=1, act='relu')
        self.p2_2 = Conv2D(num_channels=c2[0], num_filters=c2[1], filter_size=3, padding=1, act='relu')
        self.p3_1 = Conv2D(num_channels=c0, num_filters=c3[0], filter_size=1, act='relu')
        self.p3_2 = Conv2D(num_channels=c3[0], num_filters=c3[1], filter_size=5, padding=2, act='relu')
        self.p4_1 = Pool2D(pool_size=3, pool_stride=1, pool_padding=1, pool_type='max')
        self.p4_2 = Conv2D(num_channels=c0, num_filters=c4, filter_size=1, act='relu')

    def forward(self, x):
        p1 = self.p1_1(x)
        # 支路1只包含一个1x1卷积
        p2 = self.p2_2(self.p2_1(x))
        # 支路2包含 1x1卷积 + 3x3卷积
        p3 = self.p3_2(self.p3_1(x))
        # 支路3包含 1x1卷积 + 5x5卷积
        p4 = self.p4_2(self.p4_1(x))
        # 支路4包含 最大池化和1x1卷积
        return fluid.layers.concat([p1, p2, p3, p4], axis=1)
        # 将每个支路的输出特征图concat拼接在一起作为最终的输出结果

class GoogLeNet(fluid.dygraph.Layer):
    def __init__(self, num_classes=10):
        super(GoogLeNet, self).__init__()
        # GoogLeNet包含五个模块，每个模块后面紧跟一个池化层
        # 第一个模块包含1个卷积层以及3x3最大池化
        self.conv1 = Conv2D(num_channels=3, num_filters=64, filter_size=7, padding=3, act='relu')
        self.pool1 = Pool2D(pool_size=3, pool_stride=2, pool_padding=1, pool_type='max')
        # 第二个模块包含2个卷积层以及3x3最大池化
        self.conv2_1 = Conv2D(num_channels=64, num_filters=64, filter_size=1, act='relu')
        self.conv2_2 = Conv2D(num_channels=64, num_filters=192, filter_size=3, padding=1, act='relu')
        self.pool2 = Pool2D(pool_size=3, pool_stride=2, pool_padding=1, pool_type='max')
        # 第三个模块包含2个Inception块以及3x3最大池化
        self.block3_1 = Inception(192, 64, (96, 128), (16, 32), 32)
        self.block3_2 = Inception(256, 128, (128, 192), (32, 96), 64)
        self.pool3 = Pool2D(pool_size=3, pool_stride=2, pool_padding=1, pool_type='max')
        # 第四个模块包含5个Inception块以及3x3最大池化
        self.block4_1 = Inception(480, 192, (96, 208), (16, 48), 64)
        self.block4_2 = Inception(512, 160, (112, 224), (24, 64), 64)
        self.block4_3 = Inception(512, 128, (128, 256), (24, 64), 64)
        self.block4_4 = Inception(512, 112, (144, 288), (32, 64), 64)
        self.block4_5 = Inception(528, 256, (160, 320), (32, 128), 128)
        self.pool4 = Pool2D(pool_size=3, pool_stride=2, pool_padding=1, pool_type='max')
        # 第五个模块包含2个Inception块
        self.block5_1 = Inception(832, 256, (160, 320), (32, 128), 128)
        self.block5_2 = Inception(832, 384, (192, 384), (48, 128), 128)
        # 全局平均池化，尺寸用的是global_pooling，pool_stride不起作用
        self.pool5 = Pool2D(pool_stride=1, global_pooling=True, pool_type='avg')
        self.fc = Linear(input_dim=1024, output_dim=num_classes, act='softmax')

    def forward(self, x):
        x = self.pool1(self.conv1(x))
        x = self.pool2(self.conv2_2(self.conv2_1(x)))
        x = self.pool3(self.block3_2(self.block3_2(x)))
        # x = self.block4_3(self.block4_2(self.block4_1(x)))
        # x = self.pool4(self.block4_5(self.block4_4(x)))
        x = self.pool4(self.block4_5(self.block4_4(self.block4_3(self.block4_2(self.block4_1(x))))))
        x = self.pool5(self.block5_2(self.block5_1(x)))
        x = fluid.layers.reshape(x, [x.shape[0], -1])
        x = self.fc(x)
        return x
# 实际是应该有3组输出，此处简化。googlenet_paddle.py文件内详细code了所有输出