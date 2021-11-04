# PurC Fetcher

`PurC Fetcher` is a data fetcher for the PurC.


- [Source Tree of PurC Fetcher](#source-tree-of-purc-fetcher)
- [Building](#building)
   + [Commands](#commands)
- [Authors and Contributors](#authors-and-contributors)
- [Copying](#copying)
- [Tradmarks](#tradmarks)


## Source Tree of PurC Fetcher

PurC Fetcher implements the data fetcher for the PurC.
It is mainly written in C/C++ language and provides bindings for Python.

The source tree of PurC Fetcher contains the following modules:

- `Source/PurCFetcher/include/`: The global header files.
- `Source/PurCFetcher/include/private`: The internal common header files.
- `Source/PurCFetcher/utils/`: Some basic and common utilities.
- `Source/PurCFetcher/instance/`: The operations of PurCFetcher instances and sessions.
- `Source/PurCFetcher/auxiliary/`: Some auxiliary classes.
- `Source/PurCFetcher/database/`: Implement the database(sqlite) interface by C++.
- `Source/PurCFetcher/ipc/`: Interprocess communication.
- `Source/PurCFetcher/messages/`: The message templates for IPC.
- `Source/PurCFetcher/network/`: Implementation of network data fetcher
- `Source/PurCFetcher/ports/`: The ports for different operating systems, such as a POSIX-compliant system or Windows.
- `Source/WTF/`: The simplified WTF (Web Template Framework) from WebKit.
- `Source/cmake/`: The cmake modules.
- `Source/ThirdParty/`: The third-party libraries, such as `gtest`.
- `Source/test/`: The unit test programs.
- `Source/samples/`: Examples for using the interfaces of PurCFetcher.
- `tools/`: The tools (executables), e.g., the command line program.

## Building

### Commands

To build:

```
rm -rf build && cmake -DCMAKE_BUILD_TYPE=Debug -DPORT=Linux -B build && cmake --build build
```

## Authors and Contributors

- R&D Team of FMSoft (<https://www.fmsoft.cn>)

## Copying

Copyright (C) 2021 FMSoft (<https://www.fmsoft.cn>)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Tradmarks

1) `HVML` is a registered tradmark of Beijing FMSoft Technologies. Co., Ltd. in China and other contries or regions.

![HVML](https://www.fmsoft.cn/application/files/8116/1931/8777/HVML256132.jpg)

2) `呼噜猫` is a registered tradmark of Beijing FMSoft Technologies. Co., Ltd. in China and other contries or regions.

![呼噜猫](https://www.fmsoft.cn/application/files/8416/1931/8781/256132.jpg)

3) `Purring Cat` is a tradmark of Beijing FMSoft Technologies. Co., Ltd. in China and other contries or regions.

![Purring Cat](https://www.fmsoft.cn/application/files/2816/1931/9258/PurringCat256132.jpg)

4) `PurC` is a tradmark of Beijing FMSoft Technologies. Co., Ltd. in China and other contries or regions.

![PurC](https://www.fmsoft.cn/application/files/5716/2813/0470/PurC256132.jpg)

[Beijing FMSoft Technologies Co., Ltd.]: https://www.fmsoft.cn
[FMSoft Technologies]: https://www.fmsoft.cn
[FMSoft]: https://www.fmsoft.cn
[HybridOS Official Site]: https://hybridos.fmsoft.cn
[HybridOS]: https://hybridos.fmsoft.cn

[MiniGUI]: http:/www.minigui.com
[WebKit]: https://webkit.org
[HTML 5.3]: https://www.w3.org/TR/html53/
[DOM Specification]: https://dom.spec.whatwg.org/
[WebIDL Specification]: https://heycam.github.io/webidl/
[CSS 2.2]: https://www.w3.org/TR/CSS22/
[CSS Box Model Module Level 3]: https://www.w3.org/TR/css-box-3/

[Vincent Wei]: https://github.com/VincentWei
