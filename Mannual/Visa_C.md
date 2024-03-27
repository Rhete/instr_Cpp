# introduction

介绍VISA使用时需要的内容。简单介绍了VXIplug&play的功能和特性。

## VISA需要的软硬件

1. NI 基础硬件支持，包括GPIB, GPIB-VXI, MXI/VXI。同时电脑需要有足够的接口
2. 对于GPIB的应用，需要安装**NI-488**，对于VXI应用，需要安装**NI-VXI**，对于其他硬件接口，NI-VISA使用系统标准驱动
3. 发行介质（？）
4. 来自其他vendor的命令模块需要哪个vender提供的GPIB-VXI VISA内容
5. 下载最新版本，访问 <https://ni.com/visa>

## VXI*plug&play* 概述

VXI*plug&play*系统联盟旨在通过开放的、多vendor的系统来提高终端用户使用便捷。联盟成员有着实现包含软硬件的多vendor系统架构的共同愿景，因此，联盟的成员们通力合作，定义和嵌入了系统级标准。

联盟开发了一个 I/O软件规格————VISA（the Virtual Instrument System Architecture），定义了下一代I/O软件标准，不仅适用于VXI，还适用于**GPIB**，**串口**以及其他接口。VISA标准在超过**35**家大型仪器生产公司的支持下，VISA统一了全行业，使软件可互通，可重构，并且经受住了时间的考验。此外，联盟还把流行的操作系统、应用开发环境以及编程语言分类到独立的框架中，并且定义了深入规范，以保证各个框架的组建能够互通。

## 支持的平台

LabWindows/CVI，ANSI C，LabVIEW

# VISA API 概述

本手册可作为**VISA API**的参考，**VISA API**分为三个独立的机制：**属性**、**事件**、**操作**，这三个独立的机制是访问信息的途径。

## VISA 访问机制

本节总结了各个机制最重要的一些**特性**，详细信息见下一章。

### 属性

属性描述了**会话**或**资源**中反应所给对象操作状态的特征，这些属性可以通过以下操作访问：
```C
viGetAttribute()
ViSetAttribute()
```

### 事件

**事件**是独立于系统中正常顺序执行的流程的一种非同步的**发生**。取决于需要如何处理事件的发生，你可以在`viEnableEnvent()`操作中使用`viInstallHandler()`操作或`viWaitonEvent()`操作。

**事件**对**属性**的响应与**资源**相同。当应用通过一个由`viWaitOnEvent()`接受的特定**事件**完成时，需要调用`viClose()`来摧毁这个事件。

### 操作

**操作**是由可以被所给**资源**执行的**资源**定义的行为。每个资源有定义一系列操作的能力。除了被各个**资源**定义的**操作**，以下操作模板可以在任何**资源**中被使用：

```C
viClose()
viGetAttribute()
viSetAttribute()
viStatusDesc()
viTerminate()
viLock()
viUnlock()
viEnableEvent()
viDisableEvent()
viDiscardEvents()
viWaitOnEvent()
viInstallHandler()
viUninstallHandler()
```
## VISA 资源种类

目前，有各种各样的资源类型：**INSTR**, MEMacc, INTFC, BACKPLANE, SERVANT, SOCKET 和 RAW。大多数VISA应用和仪器驱动只用**INSTR 资源**。

### INSTR

**INSTR**资源能够使控制器和所给资源相关的设备交互。这类资源保证了控制者可以依据连接设备和接口类型选择使用**基于消息的输入/输出**还是**基于寄存器的输入/输出**，或是两者都用。

基本的**I/O**服务包括了向设备发送和从设备接受数据块的功能。数据的意义是依赖于设备的，可以是一条信息，一条指令或其他二进制编码的数据。对于**IEEE-488**兼容的设备，I/O服务还包括了**触发**（硬件的和软件的）、**服务请求**、**读取状态位** 和 **清除仪器**。

**格式化I/O**服务提供了与设备通信时的格式化和缓冲I/O的功能。格式化功能包括由ANSI C规定的和仪器系统使用的常见的协议。缓冲功能提高了系统的表现，可以传输大块数据和同时发送多个指令。

**内存I/O**（或**寄存器I/O**）服务允许寄存器级别的设备访问（对于支持内存直接访问的接口，如VXIbus或VMEbus）。高级和低级访问服务都提供了针对单个寄存器访问的操作，它们分别在速度和复杂性之间有取舍。高级访问服务还具有将大块数据从设备移入和移出的操作。当使用**INSTR**资源时，所有地址参数都是相对于设备在给定地址空间中分配的内存基址的；用户不需要知道设备的基址。

**内存共享**服务可以实现在一个只被给定会话使用的特定设备上分配内存。这通常只在那些专门为此目的导出共享内存的设备上可用，例如**VXIbus**或**VMEbus**控制器。

# 属性

敬请期待

# 事件

敬请期待


# 操作

## 操作目录
**API提供的所有操作**
|<center>操作名</center>|<center>操作方法</center>|操作内容|
|:-|:-|:-:|
|[`viAssertIntrSignal`](#Opt1) |`ViStatus = ViAssertIntrSignal(ViSession vi, Vint16 mode, ViUint32 statusID)`|插入给定的中断或信号
|[`viAssertTrigger`](#Opt2)|`ViStatus = viAssertTriger(Visession vi, ViUInt16 protocol)`|插入软件或硬件触发
|[`viAssertUtilSignal`](#Opt3)|`viStatus = viAssertUtilSignal(ViSession vi, ViUint16 line)`|插入或取消插入给定的通用总线信号
|[**`viBufRead`**](#Opt4)|`ViStatus viBufRead(ViSession vi, ViPBuf buf, ViUInt32 count, ViPUint32 retCount)`|通过格式化I/O读取缓存从设备或接口读取数据
|[**`viBufWrite`**](#Opt5)|`ViStatus viBufWrite(ViSession vi, ViBuff buf, ViUInt32, count, ViPUInt32 retcount)`|同步向格式化I/O写入缓存写入数据
|[**`viClear`**](#Opt6)|`ViStatus viClear(Visession vi)`|清空一个设备
|[`viClose`](#Opt7)|`ViStatus viClose(ViObject vi)`|关闭指定的会话、事件或查找表
|[`viDisableEvent`](#Opt8)|`ViStatus viDisableEvent(ViSession vi, ViEventType eventType, ViUInt16 mechanism)`|通过指定的机制取消指定事件类型的通知
|[`viDiscardEvents`](#Opt9)|`ViStatus viDiscardEvents(ViSession vi, Vi, ViEventType eventType, ViUInt16 Mechanism)`|对会话中的特定事件类型和机制抛弃事件产生
|[`viEnableEvent`](#Opt10)|`ViStatus viEnableEvent(ViSession vi, ViEventType eventType, ViUInt16 mechanism, ViEventFilter context)`|激活指定的事件的通知
|[`viEventHandler`](#Opt11)|`ViStatus _VI_FUNCH viEventHandler(ViSession vi, ViEventType eventType, ViEvent context, ViAddr userHandle)`|事件服务句柄处理协议
|[`viFindNext`](#Opt12)|`ViStatus viFindNext(ViFindList findList, ViChar instrDesc[])`|返回从上次用`viFindRsrc()`获取的资源列表的下一个资源|
|[`viFindRsrc`](#Opt13)|`ViStatus viFindRsrc(ViSession sesn, ViString expr, ViPFindList findList, ViPUInt32 retcnt, ViChar instrDesc[])`|询问VISA系统，定位经过特定的接口的资源
|[**`viFlush`**](#Opt14)|`ViStatus viFlush(ViSession vi, ViUInt16 mask)`|手动清空指定的格式化I/O操作或串口通信的缓存
|[`viGetAttribute`](#Opt15)|`ViStatus viGetAttribute(ViObject vi, ViAttr attribute, void * attrState)`|获取属性状态
|[`viGpibCommand`](#Opt16)|`ViStatus viGpibCommand(ViSession vi, ViBuf buf, ViUInt32 count, ViPUInt32 retCount)`|向总线写入GPIB命令码
|[`viGpibControlATN`](#Opt17)|`ViStatus viGpibControlATN(ViSession vi, ViUInt16 mode)`|指明ATN线和bending主动控制器的状态
|[`viGpibControlREN`](#Opt18)|`ViStatus viGpibControlREN(ViSession vi, ViUInt16 mode)`|控制GPIB的远端使能接口线，或设备的远端/本地状态（可选）
|[`viGpibPassControl`](#Opt19)|`ViStatus viGpibPassControl(ViSession vi, ViUInt16 primAddr, ViUInt16 secAddr)`|告诉指定地址的GPIB设备成为主控制器（Controller in Charge, CIC）
|[`viGpibSendIFC`](#Opt20)|`ViStatus viGpibSendIFC(ViSession vi)`|给接口清除线一个至少100毫秒的脉冲
|[`viIn8`/`viIn16`/`viIn32`](#Opt21)|`ViStatus viInt8(ViSession vi, ViUInt16 space, ViBusAddress offset, ViPUInt8 val8)`<br>`ViStatus viInt16(ViSession vi, ViUInt16 space, ViBusAddress offset, ViPUInt16 val16)`<br>`ViStatus viIn32(ViSession vi, ViUInt16 space, ViBusAddress offset, ViPUInt32 val32)`|从指定内存空间或偏移处读取8-bit、16-bit或32-bit数据
|[`viInstallHandler`](#Opt22)|`ViStatus viInstallHandler(ViSession vi, ViEventType eventType, ViHndlr handler, ViAddr userHandle)`|安装句柄，用于事件回调|
|[`viLock`](#Opt23)||
|[`viMapAddress`](#Opt24)||
|[`viMapTrigger`](#Opt25)||
|[`viMenAlloc`](#Opt26)||
|[`viMemFree`](#Opt27)||
|[`viMove`](#Opt28)||
|[`viMoveAsync`](#Opt29)||
|[`viMoveIn8`/<br>`viMoveIn16`/<br>`viMoveIn32`](#Opt30)||
|[`viMoveOut8`/<br>`viMoveOut16`/<br>`viMoveOut32`](#Opt31)||
|[**`viOpen`**](#Opt32)|`ViStatus viOpen(ViSession sesn, ViRsrc, rsrcName, ViAccessMode accessMode, ViUInt32 openTimeout, ViPSession vi)`|打开与指定资源的会话|
|[**`viOpenDefaultRM`**](#Opt33)||
|[`viOut8`/`viOut16`/<br>`viOut32`](#Opt34)||
|[`viParseRsrc`](#Opt35)||
|[`viParseRsrcEx`](#Opt36)||
|[`viPeek8`/`viPeek16`/<br>`viPeek32`](#Opt37)||
|[`viPock8`/`viPock16`/<br>`viPock32`](#Opt38)||
|[`viPrintf`](#Opt39)||
|[`viQueryf`](#Opt40)||
|[`viRead`](#Opt41)||
|[`viReadAsync`](#Opt42)||
|[`viReadSTB`](#Opt43)||
|[`viReadToFile`](#Opt44)||
|[`viScanf`](#Opt45)||
|[`viSetAttribute`](#Opt46)||
|[`viSetBuf`](#Opt47)||
|[`viSPrintf`](#Opt48)||
|[`viSScanf`](#Opt49)||
|[`viStatusDesc`](#Opt50)||
|[`viTerminate`](#Opt51)||
|[`viUninstallHandler`](#Opt52)||
|[`viUnlock`](#Opt53)||
|[`viUnmapAddress`](#Opt54)||
|[`viUnmapTrigger`](#Opt55)||
|[`viUsbControlIn`](#Opt56)||
|[`viUsbControlout`](#Opt57)||
|[`viVPrintf`](#Opt58)||
|[`viVQueryf`](#Opt59)||
|[`viVScanf`](#Opt60)||
|[`viVSPrintf`](#Opt61)||
|[`viVSScanf`](#Opt62)||
|[`viVxiCommandQuery`](#Opt63)||
|[`viWaitOnEvent`](#Opt64)||
|[`viWrite`](#Opt65)||
|[`viWriteAsync`](#Opt66)||
|[`viWriteFromFile`](#Opt67)||


## `viAssertIntrSignal`<a id="Opt1"></a>

插入具体的中断或信号

### **C code**
``` C 
Vistatus = viAssertIntrSignal(ViSession vi, ViInt16 mode, ViUInt32 statusID)
```

### 参数

||参数名||传递方向||描述||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
||vi||入||会话的唯一逻辑标识符||
||mode||入||说明如何插入终端，实际值详见***描述***部分||
||statusID||入||中断确认周期中状态值的呈现，有效值详见***描述***部分||

### 返回值

||完成代码||描述||
|:-:|:--:|:-:|:-:|:-:|
||`VI_SUCCESS`|&emsp;|成功完成操作||
||`VI_ERROR_INV_OBJECT`||目标无效||
||`VI_ERROR_NSUP_OPER`||目标不支持此操作||
||`VI_ERROR_RSRC_LOCKED`||目标资源禁止访问，无法执行操作||
||`VI_ERROR_BERR`||传输过程中总线错误||
||`VI_ERROR_INTR_PENDING`||上次请求的中断仍处于待定状态||
||`VI_ERROR_INV_MODE`||mode参数值无效||
||`VI_ERROR_NSUP_INTR`||中断要求等级过高或不满足要求的状态ID值||
||`VI_ERROR_NSUP_MODE`||VISA工具不支持给定的模式||

### 描述

此操作可用于插入设备的中断状态。例如，在VXI中，此操作即可由VXI信号执行，也可以由VXI中断执行。对于某些总线类型，`statusID`参数可以忽略。下表给出了**mode**参数的有效值。

||**Mode**||**操作描述**||
|:-:|:-:|:-:|:-:|:-:|
||`VI_ASSERT_USE_ASSIGNED`||采用本地仪器指定的通知方式||
||`VI_ASSERT_SIGNAL`||通过**VXI**信号发送通知||
||`VI_ASSERT_IRQ1` –<br>`VI_ASSERT_IRQ7`||通过指定的**VXI/VME IRQ**线路发送中断信号，该命令使用了标准的**VXI/VME ROAK**中断机制，而不是更老的**VME RORA**机制||

## `viAssertTrigger` <a id="Opt2"></a>

## `viAssertUtilSignal` <a id="Opt3"></a>

## `viBufRead` <a id="Opt4"></a>

## `viBufWrite` <a id="Opt5"></a>

## `viClear` <a id="Opt6"></a>

## `viClose` <a id="Opt7"></a>

关闭指定的会话、事件或查找表

### **C code**
```C
ViStatus viClose(ViObject vi)
```

### 参数
||参数名||传递方向||描述||
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|
||`vi`||入||会话、事件或查找表的唯一标识符||

### 返回值
||完成代码||描述||
|:-:|:-:|:-:|:-:|:-:|
||`VI_SUCCESS`|&emsp;|成功关闭会话||
||`VI_WARN_NULL_OBJECT`||指定的对象尚未初始化||

||错误代码||描述||
|:-:|:-:|:-:|:-:|:-:|
||`VI_ERROR_INV_OBJECT`|&emsp;|给定的对象引用无效||
||`VI_ERROR_CLOSING_FAILED`||无法取消分配会话或对象引用分配的数据结构||

### 描述
`viClose()`操作关闭会话、事件或查找表。这个过程中，所有被分配给**`vi`**的数据结构将被释放。针对VISA资源管理器的会话使用此操作将关闭与该资源管理器相关的所有I/O会话。

## `viDisableEvent` <a id ="Opt8"></a>



## `viDiscardEvents` <a id ="Opt9"></a>



## `viEnableEvent` <a id ="Opt10"></a>



## `viEventHandler` <a id ="Opt11"></a>



## `viFindNext` <a id ="Opt12"></a>



## `viFindRsrc` <a id ="Opt13"></a>



## `viFlush` <a id ="Opt14"></a>



## `viGetAttribute` <a id ="Opt15"></a>



## `viGpibCommand` <a id ="Opt16"></a>



## `viGpibControlATN` <a id ="Opt17"></a>



## `viGpibControlREN` <a id ="Opt18"></a>



## `viGpibPassControl` <a id ="Opt19"></a>



## `viGpibSendIFC` <a id ="Opt20"></a>



## `viIn8/viIn16/viIn32` <a id ="Opt21"></a>



## `viInstallHandler` <a id ="Opt22"></a>



## `viLock` <a id ="Opt23"></a>



## `viMapAddress` <a id ="Opt24"></a>



## `viMapTrigger` <a id ="Opt25"></a>



## `viMemAlloc` <a id ="Opt26"></a>



## `viMemFree` <a id ="Opt27"></a>



## `viMove` <a id ="Opt28"></a>



## `viMoveAsync` <a id ="Opt29"></a>



## `viMoveIn8/viMoveIn16/viMoveIn32` <a id ="Opt30"></a>



## `viMoveOut8/viMoveOut16/viMoveOut32` <a id ="Opt31"></a>



## **`viOpen`** <a id ="Opt32"></a>

打开与指定资源的会话

### **C code**
```C
ViStatus viOpen(ViSession sesn, ViRsrc, rsrcName, ViAccessMode accessMode, ViUInt32 openTimeout, ViPSession vi)
```

### 参数

## **`viOpenDefaultRM`** <a id ="Opt33"></a>



## `viOut8/viOut16/viOut32` <a id ="Opt34"></a>



## `viParseRsrc` <a id ="Opt35"></a>



## `viParseRsrcEx` <a id ="Opt36"></a>



## `viPeek8/viPeek16/viPeek32` <a id ="Opt37"></a>



## `viPoke8/viPoke16/viPike32` <a id ="Opt38"></a>



## `viPrintf` <a id ="Opt39"></a>



## `viQueryf` <a id ="Opt40"></a>



## `viRead` <a id ="Opt41"></a>



## `viReadAsync` <a id ="Opt42"></a>



## `viReadSTB` <a id ="Opt43"></a>



## `viReadToFile` <a id ="Opt44"></a>



## `viScanf` <a id ="Opt45"></a>



## `viSetAttribute` <a id ="Opt46"></a>



## `viSetBuf` <a id ="Opt47"></a>



## `viSPrintf` <a id ="Opt48"></a>



## `viSScanf` <a id ="Opt49"></a>



## `viStatusDesc` <a id ="Opt50"></a>



## `viTerminate` <a id ="Opt51"></a>



## `viUninstallHandler` <a id ="Opt52"></a>



## `viUnlock` <a id ="Opt53"></a>



## `viUnmapAddress` <a id ="Opt54"></a>

## `viUnmapTrigger` <a id ="Opt55"></a>

## `viUsbControlIn` <a id ="Opt56"></a>

## `viUsbControlOut` <a id ="Opt57"></a>

## `viVPrintf` <a id ="Opt58"></a>

## `viVQueryf` <a id ="Opt59"></a>

## `viVScanf` <a id ="Opt60"></a>

## `viVSPrintf` <a id ="Opt61"></a>

## `viVSScanf` <a id ="Opt62"></a>

## `viVxiCommandQuery` <a id ="Opt63"></a>

## `viWaitOnEvent` <a id ="Opt64"></a>

## `viWrite` <a id ="Opt65"></a>

## `viWriteAsync` <a id ="Opt66"></a>

## `viWriteFromFile` <a id ="Opt67"></a>

