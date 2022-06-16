# bazel入门1 - 从Hello World说起

本系列内容基于windows系统运行bazel，对于其它系统用户，可参考bazel[官网](https://docs.bazel.build/versions/main/bazel-overview.html)。所有运行代码已上传至[此处](https://github.com/caichangzhi/cpp-xrack-example)。

## 安装bazel

- 可从[此处](https://github.com/bazelbuild/bazel/releases)下载bazel文件，对于windows系统应下载名为`bazel-<version>-windows-x86_64.exe`的文件。

- 安装之后，可将该文件重命名为`bazel.exe`，并将其路径添加到环境变量中。

- 为确认是否安装成功，可打开PowerShell输入`bazel version`查看bazel版本以验证。若显示相关信息，则证明安装成功。

## example1

第一个项目具有如下的文件结构：

```bash
└─example1
  ├─src
  │  ├─BUILD
  │  └─hello-world.cc
  └─WORKSPACE
```

### 工作区

工作区包含要构建软件的源文件，每个工作区都有一个名为`WORKSPACE`的文本文件，该文件可能为空，也可能包含构建输出所需要的外部依赖项的引用，和其他外部代码库的使用规则。包含名为`WORKSPACE`的文件的目录被视为工作区的根目录。在本项目中所指即为`example1`文件夹。

### 包裹

包裹定义为包含名为`BUILD`的文件目录，一个包裹包含其目录中的所有文件，以及该文件下的所有子目录（包括`BUILD`文件的子目录除外）。在本项目中所指即为`src`文件夹。

### 目标

包裹是一个容器，包裹中的元素被称为目标，它在包裹中的`BUILD`文件中被定义。大部分目标为两种类型之一：文件和规则。文件进一步分为源文件和派生文件，源文件会嵌入到代码库中，派生文件不会嵌入，但是由源文件生成的。规则一般是一组输入和一组输出文件之间的关系。规则的输入一般是源文件，但也会输出文件，输出的文件一般用于其他的输入，一般用来建立较大项目。本项目中指的为`src/BUILD`文件中的`cc_binary`。

### 标签

目标的名称称为标签，每个标签都唯一标识一个目标，一般规范格式的标签为：
```
@myrepo//src:example1
```
标签的第一部分`@myrepo//`为代码库名称，通常所引用的标签在同一个代码库下，所有可以缩写为`//`。
标签第二部分为`src`为包裹路径，即在代码库中导向改包裹所在位置。
标签最后一部分的`:example1`为定位到的目标名称。
当标签以`@//`开头时引用主代码库。即使是在外部代码库中，例如`@//a/b/c`引用主代码库`//a/b/c`路径下目标，而`//a/b/c`引用外部代码库本身路径下目标。

### load

Bazel扩展名是以.bzl结尾的文件，我们一般使用load语句从外部扩展中导入符号。如本项目中的：
```
load("@rules_cc//cc:defs.bzl", "cc_binary")
```
这句话会从`rules_cc`中导入文件`/cc:defs.bzl`,在环境中添加`cc_binary`符号。load可用于加载新规则，函数或常量。

### cc_binary - ex1

使用Bazel编译C/C++代码时，`cc_binary`为不可或缺的一环。关于它的详细讲解可参考[此处](https://docs.bazel.build/versions/main/be/c-cpp.html#cc_binary)。

这里我们使用它的两个属性

- name: 生成目标的名称
- src: 创建目标所需的C/C++文件的列表

### 编译 - ex1

执行下面的命令即可执行编译：
```
bazel build //src:example1
```
- `//src`: `BUILD`文件相对于`WORKSPACE`的位置
- `example1`: `BUILD`文件中的目标名称

这时，在根目录下会生成名为`bazel-bin`的文件夹，其中`example1.exe`文件即本项目代码生成结果。

## example2

第二个项目具有如下的文件结构：

```bash
└─example2
  ├─src
  │  ├─BUILD
  │  ├─hello-greet.h
  │  ├─hello-greet.cc
  │  └─hello-world.cc
  └─WORKSPACE
```

### cc_library - ex2

使用Bazel构建C/C++库，供其他地方调用时，就需要用到`cc_library`。关于它的详细讲解可参考[此处](https://docs.bazel.build/versions/main/be/c-cpp.html#cc_library)。

这里我们使用它的三个属性：

- name: 生成目标的名称
- srcs: 创建目标所需的C/C++文件的列表
- hdrs: 该库的头文件列表，将直接包含在依赖源中

### cc_binary - ex2

除`example1`使用到的属性外，此处使用到一个新的属性：

- deps：需要链接的其他二进制库的列表

## example3

第三个项目具有如下的文件结构：

```bash
└─example3
  ├─lib
  │  ├─BUILD
  │  ├─hello-time.h
  │  └─hello-time.cc
  ├─src
  │  ├─BUILD
  │  ├─hello-greet.h
  │  ├─hello-greet.cc
  │  └─hello-world.cc
  └─WORKSPACE
```

如上所示，为了代码模块化更加清晰，此处新创建一个`lib`文件夹用于存放生成库文件的代码。

### cc_library - ex3

对于`lib`文件夹下的`BUILD`文件，除`example2`使用到的属性外，此处使用到一个新的属性：

- visibility：本目标是否可在其他包裹中使用。`__pkg__`是一种特别语法，标识包中所有目标。更多关于visibility的讲解可参考[此处](https://docs.bazel.build/versions/main/visibility.html)。

## link-opencv

在交付项目中，opencv必不可少，下面介绍如何使用Bazel从JFrog上优雅地拉下opencv库。

本项目具有如下的文件结构
```bash
└─link-opencv
  ├─src
  │  ├─BUILD
  │  └─opencv-demo.cc
  ├─WORKSPACE
  ├─BUILD
  └─opencv.BUILD
```

如该结构所示，在根目录下除了`WORKSPACE`文件，另有`BUILD`和`opencv.BUILD`文件。

### WORKSPACE - link

在上面三个项目中，根目录下的`WORKSPACE`为空，而在本项目中，`WORKSPACE`需要包含所需要的opencv库的引用。在这里，我们使用`http_archive`来构建目标。

#### http_archive

在使用`http_archive`前，需要先使用`load`将其载入：
```
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
```

`http_archive`主要功能是通过http链接下载压缩文件，并解压缩使其成为外部依赖库并作为目标可以绑定。本项目中主要用到了以下属性：

- name: 作为目标的名称
- url: 下载文件的链接
- sha256: 下载文件的预期sha256
- build_file: 针对下载文件的BUILD文件，一般命名为`目标名称.BUILD`形式

### cc_library - link

在本项目中，针对下载opencv库的`BUILD文件，使用到了如下几个特性：

- glob: glob是一个辅助函数，可用于查找与特定路径模式匹配的所有文件。路径模式匹配的规则与shell脚本类似，可参考[此处](https://docs.bazel.build/versions/main/be/functions.html#glob)
- includes: 编译时链接的的include目录
- visibility: public: 所有人都可使用此目标
- linkstatic: 在`cc_library`中，表示只允许链接静态库，所有不会产生`.dll`文件。

### package

在`BUILD`文件中，使用`package`来定义本包裹下所有目标的属性，这里使用`default_visibility`来表示该包裹下所有目标都可见。更多关于`package`的用法可参考[此处](https://docs.bazel.build/versions/main/be/functions.html)。

