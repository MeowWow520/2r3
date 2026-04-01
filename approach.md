## 实现思路


### 数据结构
| 变量名 | 描述 |
| --- | --- |
| _2DPointList | SDL下的坐标系 |
| _3DPointList | 标准正交坐标系 |

### 更新逻辑 Update()

3D 原始点 → rotateY 旋转 → 计算深度 → 透视投影 → 坐标转换 → 2D 渲染点

### 渲染逻辑 Render()

**顺序：** 
BACKGRD -> LINE -> POINT