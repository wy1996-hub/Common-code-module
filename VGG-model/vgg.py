import paddle
import paddle.fluid as fluid
from paddle.fluid.dygraph.nn import Conv2D, Pool2D, BatchNorm, Linear

# step.1 定义VGG块，包含可指定层数的卷积层和一层2x2最大池化层
# ----将conv一层一层加到block里面
class vgg_block(fluid.dygraph.Layer):
    def __init__(self, num_convs, in_channels, out_channels):
        """
        num_convs, 卷积层的数目
        num_channels, 卷积层的通道数，在同一个块内，卷积层输出通道数是一样的
        """
        super(vgg_block, self).__init__()
        self.conv_list = []
        for i in range(num_convs):
            conv_layer = self.add_sublayer('conv'+str(i),
                                           Conv2D(num_channels=in_channels, num_filters=out_channels,
                                                  filter_size=3, padding=1, act='relu'))
            self.conv_list.append(conv_layer)
            in_channels = out_channels        # 把输出的通道数赋给输入通道，以便进行下次的循环
        self.pool = Pool2D(pool_size=2, pool_type='max', pool_stride=2)

    def forward(self, x):
        for conv_iter in conv_list:
            x = conv_iter(x)
        return self.pool(x)

# step.2定义VGG网络
# ----将block一层一层加到VGG网络里面
class VGG(fluid.dygraph.Layer):
    def __init__(self, conv_arch=((2, 64), (2, 128), (3, 256), (3, 512), (3, 512)), num_classes=1000):
        super(VGG, self).__init__()
        self.vgg_blocks = []
        iter_id = 0
        # 添加vgg_block
        # 这里一共5个vgg_block，每个block里面的卷积层数目和输出通道数由conv_arch指定
        in_channels = [3, 64, 128, 256, 512, 512]
        for (num_convs, num_channels) in conv_arch:
            block = self.add_sublayer('block'+str(iter_id),
                                      vgg_block(num_convs, in_channels=in_channels[iter_id], out_channels=num_channels))
            self.vgg_blocks.append(block)
            iter_id += 1
        self.fc1 = Linear(input_dim=512*7*7, output_dim=4096, act='relu')
        self.drop_ratio1 = 0.5
        self.fc2 = Linear(input_dim=4096, output_dim=4096, act='relu')
        self.drop_ratio2 = 0.5
        self.fc3 = Linear(input_dim=4096, output_dim=num_classes, act='softmax')

    def forward(self, x):
        for block_iter in self.vgg_blocks:
            x = block_iter(x)
        x = fluid.layers.reshape(x, [x.shape[0], -1])
        x = fluid.layers.dropout(self.fc1(x), self.drop_ratio1)
        x = fluid.layers.dropout(self.fc2(x), self.drop_ratio2)
        x = fc3(x)
        return x
