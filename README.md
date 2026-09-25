# 🌾 FarmScape

> **Sometimes, the best way to optimize your life is to stop optimizing it.**

**FarmScape** is a 2D farming, ranching, and fishing simulation game developed in **C++** using the **iGraphics** framework.

The game follows **Arham**, a software engineer who becomes burned out from a life centered around constant optimization, deadlines, and city life. He leaves the city behind and moves to the countryside to start a new life through farming, ranching, and fishing.

---

## 🎮 Game Features

* **🏘️ Town Center:** Travel between different regions and access the farming, ranching, and fishing zones.

* **🌱 Level 1 — Farming:** Plant **Rice** and **Tomatoes**, track their growth phases, harvest mature crops, and manage farm output.

* **🐄 Level 2 — Ranching:** Buy and care for **Hens, Cows, and Sheep**. Feed livestock, manage lifecycle timers, and collect **Eggs, Milk, and Wool**.

* **🎣 Level 3 — Fishing:** Head to the local waters, cast your fishing line, react when a fish bites, catch different species, and sell your catch at the fish market.

* **💰 In-Game Economy:** Earn Gold by harvesting crops, collecting animal produce, and selling fish. Reinvest Gold into seeds, feed, livestock, and other resources.

* **💾 Save/Load System:** Preserve farm progress and game state across multiple save slots.

* **🎵 Interactive Controls & Audio:** Navigate using mouse and keyboard controls with background music controls available through the settings.

* **📖 Storyline:** Follow Arham's journey from an overworked software engineer to a farmer building a new life in the countryside.

---
# **🌱 Level 1 — Farming**

Arham's new life begins with the land.

Players can cultivate crops, manage their growth, harvest them, and sell the produce to earn Gold.

Features
* 🌾 Plant Rice
* 🍅 Plant Tomatoes
* 🌱 Manage crop growth
* 🧺 Harvest mature crops
* 💰 Sell harvested produce
* 🛒 Purchase seeds
* 📈 Reinvest earnings into the farm

The farming system introduces the game's basic resource-management mechanics.

<img width="787" height="592" alt="image" src="https://github.com/user-attachments/assets/170569f8-c361-4d2b-be58-d8d587bcb773" />

# **🐄 Level 2 — Ranching**

Once the farm is running, Arham can expand into livestock.

Players can purchase animals, feed them, wait for them to grow, and collect the products they produce.

Animals

* 🐔 Hens → Eggs
* 🐄 Cows → Milk
* 🐑 Sheep → Wool

Features
* Purchase livestock
* Feed animals
* Manage animal growth
* Collect animal products
* Sell produce
* Purchase additional animals
* Manage Gold and resources

The ranch adds another layer of resource management to the game.

<img width="806" height="602" alt="787760314_1084271547323854_2017972907693148324_n" src="https://github.com/user-attachments/assets/88cc8be0-70d3-4ad0-9776-1357fc0f2246" />

# **🎣 Level 3 — Fishing**

Not every day needs to be spent working the farm.

Arham can leave the fields behind and head to the water.

Level 3 introduces fishing as the third major activity in FarmScape.


The fishing system uses several gameplay states:

* **Idle** — Ready to cast.
* **Waiting** — The line has been cast and the player waits for a bite.
* **Hooked** — A fish has bitten; the player must react quickly.
* **Caught** — The fish has been successfully caught.
* **Nothing** — No fish was caught or the player reacted too slowly.

There is a **30% chance that nothing bites** and a **70% chance that a fish hooks** after the waiting period.

When a fish is hooked, the player has a limited reaction window to reel it in.

Players can:

* 🐟 Fish and catch aquatic resources
* 💰 Earn Gold from their catches
* 🌊 Take a break from farming and ranching

Fishing gives the player another way to earn resources while exploring a different environment.

<img width="797" height="589" alt="image" src="https://github.com/user-attachments/assets/c4486051-0ce6-4662-a46e-07b1fbcf0909" />

# 🛒 Market

<img width="782" height="585" alt="image" src="https://github.com/user-attachments/assets/7b690fd4-6ab4-4d4d-b757-f757f7d9789f" />

# 🏘️ Town

The Town acts as the central hub connecting the different parts of FarmScape. From the Town, players can travel between the three levels.

<img width="795" height="598" alt="image" src="https://github.com/user-attachments/assets/b8920040-9c3f-4e52-b5a4-bae4e8f5e665" />


# 💰 In-Game Economy

| Category     | Item    | Buy Price | Sell Price |
| ------------ | ------- | --------: | ---------: |
| 🌾 Crops     | Rice    |        $5 |        $10 |
| 🍅 Crops     | Tomato  |       $15 |        $20 |
| 🌾 Supplies  | Feed    |        $5 |          — |
| 🐔 Livestock | Hen     |       $30 |   Egg: $15 |
| 🐄 Livestock | Cow     |      $100 |  Milk: $30 |
| 🐑 Livestock | Sheep   |       $70 |  Wool: $45 |
| 🎣 Fishing   | Goonch  |         — |        $70 |
| 🎣 Fishing   | Perch   |         — |        $40 |
| 🎣 Fishing   | Catfish |         — |        $30 |
| 🎣 Fishing   | Tilapia |         — |        $10 |
| 🎣 Fishing   | Rui     |         — |        $30 |
| 🎣 Fishing   | Chitol  |         — |        $50 |

The player earns Gold by producing and selling resources, then uses that Gold to purchase seeds, feed, and livestock.

---

# 💾 Save & Load System

FarmScape includes a persistent save system that allows players to preserve their progress.

The save system stores game-state information and supports **multiple save slots**.

This allows players to maintain different farm progressions without losing existing saves.

---

# 🎵 Audio & Settings

The game includes background music controls.

Players can access the settings menu to manage:

* 🎵 Background music
* 🎮 Key bindings and controls
* ⚙️ Game settings

Music can be toggled during gameplay.

---

# 🕹️ Controls & Navigation

## 🖱️ Mouse Controls

Mouse interaction is used throughout the game for:

* Navigating menus
* Moving between Town, Farm, Ranch, and Fishing areas
* Opening markets
* Buying supplies
* Selling resources
* Interacting with crops
* Feeding animals
* Collecting produce
* Fishing

### Markets

Markets allow the player to buy and sell resources.

**Ranch & Town Markets**

* Sell Eggs, Milk, Wool, Crops, and other farm outputs.
* Purchase Animal Feed, Seeds, and Livestock.

**Fish Market**

* View collected fish.
* Sell fish individually for Gold.

---

## ⌨️ Tool Selection

| Key | Tool         | Function                               |
| --- | ------------ | -------------------------------------- |
| `1` | Feed Tool    | Feed ranch animals                     |
| `2` | Collect Tool | Harvest crops / collect animal produce |
| `3` | Rod Tool     | Cast fishing line in Level 3           |

---

## ⌨️ Keyboard Navigation

| Key   | Function                            |
| ----- | ----------------------------------- |
| `1`   | Menu                                |
| `2`   | Loading                             |
| `3`   | Town                                |
| `4`   | Level 1 — Farming                   |
| `5`   | Level 2 — Ranching                  |
| `6`   | Settings                            |
| `ESC` | Context-dependent back/close action |

In Level 3:

* **Left Click** — Cast fishing line
* **Left Click when hooked** — Reel in the fish
* **ESC** — Return to Town / close the Fish Market

---

# 🏘️ Game World

```text
                 ┌──────────────┐
                 │     TOWN     │
                 └──────┬───────┘
                        │
              Choose an activity
                        │
        ┌───────────────┼───────────────┐
        ▼               ▼               ▼
     🌱 FARM          🐄 RANCH       🎣 FISHING
        │               │               │
      Grow            Raise           Catch
      Crops           Animals          Fish
        │               │               │
        └───────────────┼───────────────┘
                        ▼
                   Sell Goods
                        │
                        ▼
                     💰 Gold
                        │
                        ▼
                 Buy Resources
                        │
                        ▼
                 Continue Playing
```
---

# 🛠️ Project Structure

```text
FarmScapes/
│
├── IMain.cpp             # Main entry point, game loop, and state machine
│
├── drawTown.h            # Town hub rendering and navigation logic
├── drawlevel1.h          # Level 1 — Farming graphics and mechanics
├── drawlevel2.h          # Level 2 — Ranching rendering and animal management
├── drawlevel3.h          # Level 3 — Fishing mechanics and water UI
│
├── storyline.h           # Storyline dialogs, intro sequences, and narratives
│
├── saveSystem.h          # Game-state serialization and save-slot handling
│
├── loading.h             # Level 1 loading screen
├── loading2.h            # Level 2 loading transition
├── loading3.h            # Level 3 loading transition
│
├── menu.h                # Main menu UI rendering
├── settings.h            # In-game settings and key bindings
├── toggleMusic.h         # Audio control and music initialization
│
├── IGraphics.h           # Core iGraphics GUI wrapper
│
└── save_slot_1.txt       # Example game-state save file
```

### Core Systems

| File            | Responsibility                                            |
| --------------- | --------------------------------------------------------- |
| `IMain.cpp`     | Main game loop, input handling, and game-state management |
| `drawTown.h`    | Town rendering and navigation                             |
| `drawlevel1.h`  | Farming gameplay and rendering                            |
| `drawlevel2.h`  | Ranching gameplay and animal management                   |
| `drawlevel3.h`  | Fishing gameplay and Fish Market                          |
| `storyline.h`   | Storyline and dialogue sequences                          |
| `saveSystem.h`  | Save/load functionality and save slots                    |
| `menu.h`        | Main menu                                                 |
| `settings.h`    | Settings and key bindings                                 |
| `toggleMusic.h` | Background music controls                                 |
| `loading.h`     | Farming loading transition                                |
| `loading2.h`    | Ranching loading transition                               |
| `loading3.h`    | Fishing loading transition                                |
| `IGraphics.h`   | iGraphics functionality                                   |

---

# 🧩 Game Architecture

FarmScape uses a **game-state-based architecture** to manage different screens and gameplay areas.

```text
                    ┌──────────┐
                    │   MENU   │
                    └────┬─────┘
                         │
                    ┌────▼─────┐
                    │  LOADING │
                    └────┬─────┘
                         │
                    ┌────▼─────┐
                    │   TOWN   │
                    └────┬─────┘
                         │
          ┌──────────────┼──────────────┐
          ▼              ▼              ▼
      ┌────────┐     ┌────────┐     ┌────────┐
      │ FARM   │     │ RANCH  │     │ FISHING│
      └────────┘     └────────┘     └────────┘
          │              │              │
          └──────────────┼──────────────┘
                         ▼
                      TOWN
```

Individual systems are separated into header files, while the main entry point manages the overall game loop and state transitions.

---

# 🚀 Building & Running

## Prerequisites

* **Windows**
* **Visual Studio**
* C/C++ Desktop Development tools
* **iGraphics**
* **OpenGL**
* **GLUT**
* Win32 API support

## Required Libraries

The project requires:

```text
winmm.lib
opengl32.lib
glu32.lib
```

## Setup

1. Clone the repository:

```bash
git clone https://github.com/RahmanRagib/FarmScapes.git
```

2. Open the project in **Visual Studio**.

3. Make sure the iGraphics, OpenGL, and GLUT dependencies are correctly configured.

4. Build the project in:

```text
x86 / Win32
```

5. Run the game.

---

# 📸 Screenshots


<img width="795" height="597" alt="image" src="https://github.com/user-attachments/assets/5280e980-f41b-4edc-a051-14e3aea0fc3d" />
<img width="793" height="546" alt="image" src="https://github.com/user-attachments/assets/ca3769d1-0b6d-4daa-8dba-d35248c7f07d" />

<img width="780" height="505" alt="image" src="https://github.com/user-attachments/assets/c7760e7b-fe4d-436f-94f7-e7fdfffe8ed5" />
<img width="732" height="555" alt="image" src="https://github.com/user-attachments/assets/fd2d3a53-a3f5-420a-8209-2dbd529dccf7" />


---

# 🌅 Design Philosophy

FarmScape is more than a farming simulator.

The game's central idea is the contrast between Arham's old life and his new one.

The Old Life
* ⌨️ Code
* ☕ Coffee
* ⏰ Deadlines
* 📱 Notifications
* 📊 Productivity
* 🔁 Repeat
  
The New Life
* 🌱 Plant
* 🐄 Feed
* 🎣 Fish
* 🌄 Explore
* 🏡 Build
* 🌅 Relax

The game explores a simple question:

What happens when you stop trying to optimize every moment of your life?

# 🎯 Objective

Build a sustainable countryside life for Arham.

Grow crops.

Raise animals.

Go fishing.

Earn Gold.

Expand your farm.

But most importantly, discover a different way of living.

There is no need to rush.

The countryside is waiting. 🌾

---

# 👥 Project

**FarmScape** was created as a collaborative game-development project for the course CSE1200 using **C and iGraphics**.

# Project Contributors

1. Rahman Ragib
2. Nabiha Anika
3. Nadira Fairuza

### Project Report

### Youtube Link

### Project Architecture

[FarmScapes on GitDiagram](https://gitdiagram.com/rahmanragib/farmscapes?utm_source=chatgpt.com)

---
