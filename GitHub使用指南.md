# GitHub 使用指南 · 从"能打开"到"用得熟"

这份指南专门讲：**仓库推上去之后，你打开 GitHub 该怎么操作**，怎么快速找到你要的代码，
以及以后怎么把新写的代码继续同步上去。

---

## 一、第一次打开仓库，你看到的每一块是什么

打开 `github.com/hansen-cmd/仓库名`，页面从上到下：

```
┌─────────────────────────────────────────────────┐
│  hansen-cmd / 仓库名    [Watch] [Fork] [Star]    │ ← 仓库标题栏
├─────────────────────────────────────────────────┤
│ [Code] [Issues] [Pull requests] [Actions] ...    │ ← 功能标签页
├─────────────────────────────────────────────────┤
│  [main ▾] [15 Commits]        [ Go to file ]     │ ← 分支/提交数/文件搜索
├─────────────────────────────────────────────────┤
│  📁 01-GPIO基础输出                              │
│  📁 02-按键与传感器输入          ← 文件列表      │
│  📁 03-OLED显示                                  │
│  ...                                            │
│  📄 README.md                                    │
├─────────────────────────────────────────────────┤
│  # STM32F103 入门实验合集                        │
│  （README.md 的内容自动渲染在这里）   ← 重点！   │
└─────────────────────────────────────────────────┘
```

**关键认知：文件列表下面那段内容，就是 `README.md` 渲染出来的。**
所以你在任何一个文件夹里放 `README.md`，GitHub 都会自动把它显示在那个文件夹页面的最下方
——这就是为什么我给每个项目都写了说明文档。

### 各功能标签页都是干什么的

| 标签 | 用途 | 你现在需要吗 |
|---|---|---|
| **Code** | 浏览代码（**99% 时间都在这**） | ✅ |
| **Issues** | 记待办、记 bug | 可以用它记"下一步要做什么" |
| **Pull requests** | 合并代码改动（单人开发基本用不上） | 暂时不用 |
| **Actions** | 自动化（自动编译等） | 暂时不用 |
| **Projects** | 看板管理 | 可选 |
| **Insights** | 提交统计、贡献图 | 看看自己的提交记录 |
| **Settings** | 仓库设置（改名、删库、设为私有） | 需要时 |

---

## 二、怎么找代码（最常用，务必学会）

### 方法 1：逐层点进去（适合"我知道大概在哪"）

首页点 `06-PWM与电机控制` → 点 `6-1PWM驱动LED呼吸灯` → 点 `user` → 点 `main.c`

**每进入一个文件夹，先看页面下方自动显示的说明**，比直接翻代码快得多。

### 方法 2：Go to file（适合"我记得文件名"）

在仓库页面按键盘 **`/`** 斜杠键，或点右上角 `Go to file` 框，输入文件名片段：

```
输入 main      → 列出所有含 main 的文件
输入 OLED      → 列出 OLED.c / OLED.h / OLED_Font.h
输入 uvprojx   → 列出所有 Keil 工程文件
```

选中回车直接跳转。**这是最快的文件导航方式。**

### 方法 3：全文搜索（适合"我想找某个函数在哪用的"）

在仓库页面**顶部的搜索框**（最上面那条，不是 Go to file）输入：

| 想找什么 | 输入 |
|---|---|
| 某个函数用在哪 | `PWM_SetCompare1` |
| 某个引脚在哪配置 | `GPIO_Pin_15` |
| 某个定时器 | `TIM_EncoderMode` |
| 限定在某个目录 | `PWM path:06-PWM与电机控制` |
| 只要 C 文件 | `IC_GetFreq language:c` |
| 包含中文注释的 | `呼吸灯` |

搜索结果页左侧可以再筛选语言、路径。**搜索框支持中文。**

> ⚠️ 注意区分两个搜索框：
> - **顶部全局搜索框**（带 GitHub 标志）= 全站搜索，这里搜是搜整个 GitHub
> - **仓库页面内的搜索框** = 只搜当前仓库
>
> 想在全站搜索时限定自己的仓库，用 `repo:hansen-cmd/仓库名 关键词` 语法。

### 方法 4：在文件里搜（Ctrl+F）

打开某个 `.c` 文件后，按 `Ctrl+F` 在当前文件内查找。
代码行号可以点击，**点行号会选中该行并在地址栏加上 `#L行号`**，可以复制这个链接分享给
别人（比如发给 AI 时说"看这一行"）。

---

## 三、看代码的几种姿势

| 我想干什么 | 怎么操作 |
|---|---|
| 看某个文件以前是什么样 | 打开文件 → 右上角 **History** → 点某个提交看当时的版本 |
| 看某一行是谁在什么时候改的 | 打开文件 → 右上角 **Blame** → 每行前面显示提交信息 |
| 看纯文本（方便复制） | 打开文件 → 右上角 **Raw**（得到纯文本链接） |
| 下载单个文件 | 打开 Raw 后右键另存，或在文件页右键 "Download" |
| 下载整个仓库 | 绿色 **Code** 按钮 → **Download ZIP** |
| **在网页版 VS Code 里打开** | 在仓库页面按键盘 **`.`** 句号键 ⭐ 强烈推荐 |

> **按 `.` 键** 会在浏览器里打开一个完整的 VS Code 编辑器，可以直接浏览、搜索、甚至改代码。
> 临时想看代码又不想 clone 时，这是最舒服的方式。

---

## 四、以后怎么把新代码同步上去

### 场景 A：在电脑上写完新代码，想推上去

在 `C:\Users\19284\Desktop\32练习` 文件夹里**右键 → Git Bash Here**，然后：

```bash
git status                              # 看看改了哪些文件
git add .                               # 把所有改动加入暂存区
git commit -m "完成 7-1 ADC 采集实验"    # 提交，写清楚这次做了什么
git push                                # 推送到 GitHub
```

**四步走，日常就是这四条命令。**

### 场景 B：换台电脑 / 重装系统后把代码拉回来

```bash
git clone https://github.com/hansen-cmd/仓库名.git
```

之后的代码改动同样用 `git add / commit / push` 三步同步。

### 场景 C：在 GitHub 网页上直接改（比如改 README）

1. 打开要改的文件
2. 点右上角的**铅笔图标**（Edit this file）
3. 改完拉到页面底部，填 `Commit changes` 的说明
4. 点绿色按钮提交

改完记得下次在本地先执行 `git pull` 把网页上的改动拉下来。

---

## 五、常用命令速查表

```bash
git status              # 当前改了什么（最常用，随时敲）
git add .               # 暂存所有改动
git add 文件名          # 只暂存某一个文件
git commit -m "说明"     # 提交到本地
git push                # 推到 GitHub
git pull                # 把 GitHub 上的新改动拉下来
git log --oneline       # 看提交历史（简洁版）
git diff                # 看具体改了哪些内容（还没 add 的）
```

**黄金习惯**：`git status` 随手敲，它会告诉你下一步该干什么，不用背命令。

---

## 六、几个让仓库更好用的小设置

| 想做的事 | 在哪 |
|---|---|
| 给仓库加简介和标签（别人一眼知道这是什么） | 仓库首页右上角 ⚙️（About 旁边的齿轮）|
| 加标签 `stm32` `embedded` `keil` | 同上，填 Topics |
| 把 README 钉在首页（现在已经是了） | 自动生效 |
| 设为私有（不想给别人看） | Settings → Danger Zone → Change visibility |
| 看自己的提交热力图 | 点右上角头像 → Your profile |

> 💡 **建议加这几个 Topics**：`stm32` `stm32f103` `embedded` `keil` `iot` `learning`
> 这样别人搜 STM32 项目时更容易找到你的仓库，对你未来找实习也有帮助——
> **这个仓库本身就是你的作品集。**

---

## 七、把仓库变成你的简历加分项

既然这是你从零学 STM32 的完整记录，别浪费：

1. **保持持续提交** —— 绿色的提交热力图本身就是"我确实在坚持"的证据
2. **每个项目都有说明文档** —— 面试官看到 README 写得清楚，印象完全不同
3. **后续往里加东西** —— FreeRTOS、联网上云、RFID 作品，一个仓库体现成长曲线
4. **简历上直接放仓库链接** —— 比写"熟悉 STM32"有说服力一百倍

---

## 八、遇到问题怎么办

| 现象 | 原因和解决办法 |
|---|---|
| `git push` 提示输入用户名密码 | 别输密码，用户名填 `hansen-cmd`，**密码填 PAT**（见下方说明） |
| 提示 `Permission denied` | token 过期或权限不够，重新生成一个 |
| 提示 `failed to push some refs` | GitHub 上有你本地没有的改动，先 `git pull` 再 push |
| 提交里有不该有的文件 | 检查 `.gitignore`，然后用 `git rm --cached 文件名` 移除跟踪 |
| 想撤销还没 commit 的改动 | `git checkout -- 文件名`（谨慎，改动会丢失） |
| `git push` 卡住不动，最后报 `Connection reset` / `Failed to connect` | **不是你的问题，是 DNS 被劫持了**，见第九节 |

> **关于 PAT（Personal Access Token）**：
> GitHub 从 2021 年起不再接受账号密码做命令行认证，必须用 token。
> token 相当于一个专用的、可随时吊销的钥匙，**不要共享给别人**。
> 生成位置：GitHub → 右上角头像 → Settings → Developer settings →
> Personal access tokens → Tokens (classic) → Generate new token → 勾选 `repo`。
>
> ⚠️ **别生成 fine-grained token**（就是 `github_pat_` 开头的那种）。
> 它默认**只有读权限**，上传时会被拒绝（`403 Resource not accessible`）。
> 如果非要用，必须展开 **Repository permissions → Contents**，改成 **Read and write**。

---

## 九、git 连不上 github.com（DNS 劫持）

### 症状

浏览器能打开 GitHub，`curl api.github.com` 也有响应，但 `git push` / `git clone`
要么卡住几十秒，要么报：

```
fatal: unable to access 'https://github.com/...':
Connection was reset  /  Failed to connect to github.com port 443
```

### 原因

在命令行里查一下就知道了：

```
nslookup github.com
```

如果返回里有 **`10.101.x.x`** 这类内网地址，说明 DNS 把 GitHub 解析到了一个
根本不存在的内网 IP，TCP 握手必然超时。（校园网、某些宽带会这么干。）

### 解决办法：让 hosts 文件直接指定真实 IP

仓库里已经放好了 `fix-github-hosts.bat`。

1. 找到这个文件，**右键 → 以管理员身份运行**（必须管理员，否则改不了 hosts）
2. 看到 `Done.` 就成功了

它会做三件事：备份原 hosts → 清掉旧的 github 条目 → 写入验证过的真实 IP。

### 想手动改的话

用管理员权限打开记事本，编辑这个文件：

```
C:\Windows\System32\drivers\etc\hosts
```

在末尾加上：

```
20.205.243.166 github.com
140.82.114.4   github.com
20.205.243.168 api.github.com
```

保存后执行 `ipconfig /flushdns` 刷新缓存。

> IP 会变，哪天又连不上了，去 <https://www.ipaddress.com/site/github.com>
> 查最新的，或者重新跑一遍脚本前先 `ping` 测一下。

### 验证是否修好

```
curl -v https://github.com
```

能返回 `HTTP/2 200` 就说明通了，之后再 `git push` 就正常了。

---

## 十、让本地仓库和 GitHub 对齐（只需做一次）

这个仓库最初是用 GitHub API 直接写入的，所以**你电脑上 `.git` 里的提交记录
和 GitHub 上的不是同一条历史**。内容完全一样，但 git 认的是哈希，它会认为
两者无关。

**不处理的话**，你下次 `git push` 会报：

```
! [rejected] main -> main (non-fast-forward)
```

### 解决办法

先按第九节修好网络，然后在这个文件夹里打开终端，执行：

```
git fetch origin
git reset --hard origin/main
```

这两条的意思是：把 GitHub 上的最新记录拉回来，然后让本地**以 GitHub 为准**。

> 因为两边文件内容完全一致，这个操作**不会丢任何代码**。
> 如果你在这之后自己改过代码，请先备份再执行。

### 验证

```
git log --oneline
git status
```

`git status` 显示 `nothing to commit, working tree clean`，并且
`git log` 的第一条和 GitHub 网页上看到的最新提交一致，就对齐了。

之后就可以正常 `git add` → `git commit` → `git push` 了。
