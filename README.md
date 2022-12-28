# TessngDB

## 简介

Tessng增量保存



## 存在问题

- 数据库记录，路径内存在删除的情况下，路段序列仍在界面上

```
gpScene->removeGRouting(routing)
```

- 如果路径删除后续路段序列后，仅剩下一个路段，是否需要删除路径
- 删除完路径的路段序列，路径车道连接相关数据库和内存数据（mlOneRouting，mhLCStruct）之后，仿真正常，但渲染异常，路径车道连接和被删序列仍存在



## 期望

- 在**GConnector**中开放一个接口：删除车道连接
