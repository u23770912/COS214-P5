# Greenhouse Management System - Setup & User Guide

## Table of Contents
1. [System Requirements](#system-requirements)
2. [Installation](#installation)
3. [Building the Project](#building-the-project)
4. [Running the System](#running-the-system)
5. [Usage Guide](#usage-guide)
6. [Troubleshooting](#troubleshooting)

---

## System Requirements

### Hardware
- **CPU**: Modern multi-core processor (2+ cores recommended)
- **RAM**: Minimum 2GB RAM
- **Storage**: 50MB free space

### Software
- **Operating System**: Linux (Ubuntu 20.04+ recommended) or WSL2 on Windows
- **C++ Compiler**: g++ with C++11 support
- **Java**: JDK 8 or higher
- **Libraries**:
  - GTK+ 3.0 development libraries
  - pthread (usually included with g++)
  - Standard C++ libraries

### Network
- Port 8765 must be available for GUI-backend communication

---

## Installation

### 1. Install Dependencies (Ubuntu/Debian)

```bash
# Update package list
sudo apt update

# Install C++ compiler and build tools
sudo apt install -y build-essential g++ make

# Install GTK+ 3 development libraries
sudo apt install -y libgtk-3-dev pkg-config

# Install Java Development Kit
sudo apt install -y default-jdk

# Verify installations
g++ --version        # Should show g++ 7.0 or higher
java -version        # Should show Java 1.8 or higher
javac -version       # Should show javac 1.8 or higher
pkg-config --modversion gtk+-3.0  # Should show GTK+ version
```

### 2. Clone/Copy Project Files

```bash
# Navigate to your desired directory
cd ~/Documents

# If using git
git clone https://github.com/u23770912/COS214-P5.git
cd COS214-P5

# OR copy the project folder manually
# Then navigate to it
cd /path/to/COS214/Prac5
```

### 3. Verify Project Structure

Ensure your project directory contains:
```
COS214/Prac5/
├── Makefile
├── main.cpp
├── *.cpp (all source files)
├── *.h (all header files)
├── NetBeansGUI/
│   └── src/
│       └── greenhouse/
│           ├── MainGUI.java
│           └── GreenhouseAPI.java
├── README.md
└── SETUP_GUIDE.md (this file)
```

---

## Building the Project

### Build C++ Backend

```bash
# Navigate to project root
cd /path/to/COS214/Prac5

# Clean previous builds (optional)
make clean

# Compile the C++ backend
make

# Verify successful compilation
ls -lh greenhouse
# Should show an executable file named 'greenhouse'
```

**Expected output:**
```
g++ -g -std=c++11 -Wall -I. ... (compilation messages)
g++ -o greenhouse ... (linking)
```

### Build Java GUI

```bash
# Navigate to GUI source directory
cd NetBeansGUI/src

# Compile Java files
javac greenhouse/*.java

# Verify compilation (no output = success)
# Check for .class files
ls greenhouse/*.class
```

**Expected files:**
- `MainGUI.class`
- `GreenhouseAPI.class`
- `GreenhouseAPI$Plant.class`
- `GreenhouseAPI$Worker.class`
- `GreenhouseAPI$Stats.class`

---

## Running the System

The system has **TWO MODES**:

### Mode 1: Integrated Mode (Recommended) - C++ Backend + Java GUI

This mode provides a graphical user interface for real-time interaction.

#### Step 1: Start C++ Backend

```bash
# Terminal 1: Navigate to project root
cd /path/to/COS214/Prac5

# Run the greenhouse executable
./greenhouse

# When prompted, select mode:
# Enter: 1
```

**You should see:**
```
╔════════════════════════════════════════════════════════════╗
║   GREENHOUSE MANAGEMENT SYSTEM - LAUNCH MODE SELECTION     ║
╚════════════════════════════════════════════════════════════╝

Select launch mode:
  1. Integrated Mode - C++ Backend + Java GUI (RECOMMENDED)
  2. Console Simulation Mode

Enter choice (1-2): 1

[INFO] GUI server started on port 8765
[INFO] Waiting for GUI connection...
```

#### Step 2: Start Java GUI (in a separate terminal)

```bash
# Terminal 2: Navigate to GUI directory
cd /path/to/COS214/Prac5/NetBeansGUI/src

# Run the Java GUI
java -cp . greenhouse.MainGUI
```

**The GUI window should appear with 5 tabs:**
1. Plant Monitor
2. Worker Management
3. Customer Orders
4. Dashboard
5. Live Feed

#### Step 3: Use the System

**Initial Setup:**
1. Go to **Worker Management** tab
   - Click "Add Worker"
   - Add at least one Gardener and one Cashier
   
2. Go to **Plant Monitor** tab
   - Click "Add Plant"
   - Select species (Rose, Tulip, etc.)
   - Plant type is automatically determined
   - Add several plants

**Plant Lifecycle:**
- Plants automatically progress through states:
  - Planted → InNursery → Growing → ReadyForSale
- Watch progress in Plant Monitor tab
- View status updates in Live Feed

**Customer Orders:**
1. Go to **Customer Orders** tab
2. Click "Refresh Inventory" to see available plants
3. Select a plant and pot type
4. Click "Add to Cart"
5. Click "Checkout" to complete purchase

**System Management:**
- **Dashboard** tab: View statistics and print structure
- Click **"Exit System"** to gracefully shut down both backend and GUI

---

### Mode 2: Console Simulation Mode

This mode runs an automated simulation in the terminal only.

```bash
# Navigate to project root
cd /path/to/COS214/Prac5

# Run the greenhouse executable
./greenhouse

# When prompted, select mode:
# Enter: 2

# Follow the console prompts
# Press Enter to advance through simulation phases
```

**Simulation Flow:**
1. System initialization
2. Greenhouse lifecycle simulation (plants grow automatically)
3. Customer order simulation (interactive menu)
4. System cleanup

---

## Usage Guide

### Adding Plants via GUI

1. **Open Plant Monitor Tab**
2. **Click "Add Plant" button**
3. **Fill in details:**
   - **Species**: Choose from dropdown (Rose, Tulip, Orchid, etc.)
   - **Type**: Automatically set based on species
   - **Price**: Default is $25.00 (can be changed)
4. **Click OK**
5. **Plant appears in the table and starts lifecycle**

**Plant Types:**
- **Flowers**: Rose, Tulip, Orchid, Lily, Sunflower, Lavender (20s per state)
- **Trees**: Bonsai, Oak, Maple, Pine, Willow (30s per state)
- **Succulents**: Aloe, Cactus, Echeveria, Jade (25s per state)

### Adding Workers via GUI

1. **Open Worker Management Tab**
2. **Click "Add Worker" button**
3. **Enter worker details:**
   - **Name**: Worker's name
   - **Role**: Select "Gardener" or "Cashier"
4. **Click OK**

**Worker Roles:**
- **Gardener**: Handles plant care (watering, fertilizing, pruning)
- **Cashier**: Handles sales and order processing

### Processing Customer Orders

1. **Go to Customer Orders Tab**
2. **Refresh Inventory** to see available plants (ReadyForSale state)
3. **Browse Bouquet Suggestions** (optional):
   - Select event type (Birthday, Wedding, etc.)
   - View suggested arrangements
4. **Add Plants to Cart:**
   - Select a plant from inventory table
   - Choose pot type (Clay, Plastic, Wood, Glass, Metal)
   - Click "Add to Cart"
5. **View Cart** - plants are reserved during shopping
6. **Checkout:**
   - Review cart contents
   - Click "Checkout"
   - Order is completed and plants are removed from inventory

### Monitoring System Status

**Dashboard Tab Features:**
- **Refresh Stats**: View current statistics
  - Total plants in system
  - Healthy plants count
  - Total orders processed
  - Revenue generated
- **Print Structure**: Prints greenhouse hierarchy to backend terminal
- **Exit System**: Gracefully shutdown the entire system

**Live Feed Tab:**
- Real-time event log
- Shows all system activities
- Color-coded messages (info, warnings, errors)

---

## Architecture & Design Patterns

### System Components

1. **C++ Backend** (main.cpp + supporting files)
   - Plant lifecycle simulation
   - Staff management (Chain of Responsibility)
   - Inventory management (Singleton)
   - Network server (socket communication)

2. **Java GUI** (MainGUI.java + GreenhouseAPI.java)
   - Swing-based graphical interface
   - Socket client for backend communication
   - Real-time data visualization

3. **Communication Protocol**
   - TCP socket on port 8765
   - Text-based command/response protocol
   - Commands: GET_PLANTS, ADD_PLANT, CREATE_ORDER, etc.

### Design Patterns Used

- **Singleton**: InventoryManager, BouquetSuggestionFactory
- **Prototype**: Command pattern for plant care
- **Chain of Responsibility**: Staff command handling
- **Observer**: Plant state change notifications
- **Builder**: Order construction
- **Facade**: OrderUIFacade for discount calculations
- **Adapter**: Multiple payment method integration
- **Template Method**: Bouquet suggestion generation
- **Composite**: Greenhouse structure hierarchy
- **Memento**: Order state save/restore (planned)

---

## Troubleshooting

### Compilation Issues

**Problem:** `gtk+-3.0: command not found`
```bash
# Solution: Install GTK+ development libraries
sudo apt install libgtk-3-dev pkg-config
```

**Problem:** `undefined reference to pthread_create`
```bash
# Solution: Already handled in Makefile with -pthread flag
# If issue persists, verify g++ version supports C++11
g++ --version
```

**Problem:** Java class files not found
```bash
# Solution: Ensure you're in the correct directory
cd NetBeansGUI/src
javac greenhouse/*.java
java -cp . greenhouse.MainGUI
```

### Runtime Issues

**Problem:** "Failed to connect to greenhouse server"
```bash
# Solution 1: Start C++ backend first
./greenhouse
# Select mode 1
# Wait for "GUI server started" message

# Solution 2: Check if port 8765 is available
netstat -tuln | grep 8765

# Solution 3: Check firewall settings
sudo ufw allow 8765/tcp

#Solution 4: Kill the terminals
pkill -9 greenhouse; pkill -9 java; sleep 2; netstat -tuln 2>/dev/null | grep 8765 || ss -tuln 2>/dev/null | grep 8765 || echo "Port is free"
[1]-  Killed                  ./greenhouse
[2]+  Killed                  ./greenhouse
```

**Problem:** GUI window doesn't appear
```bash
# Solution 1: Check Java GUI output for errors
java -cp . greenhouse.MainGUI

# Solution 2: Verify X11 forwarding (if using SSH)
echo $DISPLAY

# Solution 3: Try running with Java 8
sudo update-alternatives --config java
```

**Problem:** Plants not appearing in inventory
```bash
# Solution: Plants must reach "ReadyForSale" state
# Wait for lifecycle progression (20-30 seconds per state)
# Check Plant Monitor tab for current state
```

**Problem:** "Segmentation fault" on exit
```bash
# Solution: This was fixed in the latest version
# Ensure you have the latest code with removeWithoutDelete() method
# Recompile: make clean && make
```

### Memory Leak Testing

To verify no memory leaks:

```bash
# Install valgrind
sudo apt install valgrind

# Run with valgrind
make valgrind

# Or manually:
valgrind --leak-check=full --show-leak-kinds=all ./greenhouse
```

**Expected result:**
```
LEAK SUMMARY:
   definitely lost: 0 bytes in 0 blocks
   indirectly lost: 0 bytes in 0 blocks
   possibly lost: 368 bytes in 1 blocks  ← Thread-related (acceptable)
   still reachable: ~50KB  ← Static objects (acceptable)

ERROR SUMMARY: 0 errors
```

---

## Performance Notes

### System Capacity

- **Plants**: Tested up to 100 simultaneous plants
- **Workers**: Recommended 2-10 workers per team
- **Orders**: No practical limit on order history

### Timing Parameters

- **Plant State Duration**: 20-30 seconds per state
- **Simulation Timeout**: 300 seconds (5 minutes)
- **GUI Update Interval**: 1 second
- **Worker Task Duration**: 5 seconds

---

## Additional Resources

### File Structure Reference

```
Key Files:
├── main.cpp                    # Main entry point, simulation logic
├── InventoryManager.cpp/.h     # Singleton inventory management
├── PlantProduct.cpp/.h         # Plant lifecycle implementation
├── GreenhouseManager.cpp/.h    # Greenhouse structure management
├── StaffManager.cpp/.h         # Staff coordination
├── NetworkServer.cpp/.h        # Socket server for GUI
├── BouquetSuggestionFactory.cpp/.h  # Bouquet templates
└── NetBeansGUI/src/greenhouse/
    ├── MainGUI.java            # Main GUI window
    └── GreenhouseAPI.java      # Backend communication API
```

### Command Protocol Reference

**C++ Backend Commands:**
- `GET_PLANTS` - Retrieve all plants in system
- `GET_INVENTORY` - Get plants available for sale
- `GET_STATS` - Get system statistics
- `GET_WORKERS` - List all workers
- `ADD_PLANT <species> <type> <pot> <price>` - Create new plant
- `ADD_WORKER <name> <role>` - Hire new worker
- `ADVANCE <plantId>` - Force lifecycle advancement
- `SUGGEST_BOUQUET <eventType>` - Get bouquet suggestions
- `CREATE_ORDER <plantId1> <plantId2> ...` - Complete purchase
- `PRINT_STRUCTURE` - Display greenhouse hierarchy
- `EXIT` - Graceful shutdown

---