<!-- 起始时间15:26 -->
# HW7 中位数

## 实现选择
对于linear select和quick select，都在区间小于等于Q时采用 `std::ranges::nth_element` 。

quick select使用区间第一个元素作为划分依据。

## 数据集、实验方法以及对应的实验思路
依次实验N=100, 1000, 2000。对于每个N，构造长度为N的顺序序列与随机序列，然后测试Q从5到11，对于每个Q、每个输入，随机取1000次linear select、quick select，然后输出linear select在顺序、乱序下使用时间的90%分位数、quick select在顺序、乱序下使用时间的90%分位数。

编译使用C++23标准 `-O2` 优化。

## 测试结果与分析
### 测试结果
N=100:

Q|linear顺序|linear乱序|quick顺序|quick乱序
---|---|---|---|---
5|1642ns|1379ns|3914ns|448ns
6|2019ns|2117ns|4000ns|736ns
7|1256ns|1167ns|3870ns|413ns
8|1273ns|1222ns|3881ns|408ns
9|1175ns|940ns|3870ns|395ns
10|1109ns|930ns|3810ns|385ns
11|1105ns|826ns|3826ns|373ns

N=1000:

Q|linear顺序|linear乱序|quick顺序|quick乱序
---|---|---|---|---
5|15435ns|27687ns|227945ns|9426ns
6|13272ns|31630ns|228364ns|9368ns
7|12176ns|27597ns|228590ns|9295ns
8|11564ns|31769ns|228910ns|9257ns
9|10752ns|28651ns|228549ns|9201ns
10|9952ns|29355ns|229436ns|9242ns
11|10352ns|26912ns|227852ns|9170ns

N=2000:

Q|linear顺序|linear乱序|quick顺序|quick乱序
---|---|---|---|---
5|28613ns|56162ns|844207ns|16272ns
6|24097ns|60891ns|847345ns|16419ns
7|19957ns|58303ns|855685ns|16219ns
8|20309ns|66171ns|856027ns|16370ns
9|19049ns|58016ns|852676ns|16284ns
10|18107ns|61850ns|850737ns|16245ns
11|17349ns|56616ns|842824ns|16052ns


### 结果分析
在顺序数据上，linear select的90%分位运行时间普遍优于quick select。但在乱序数据上，quick select性能普遍更优。

<!-- 结束时间16:31 -->