# COS 214 Project - Greenhouse Management System

## ✅ RECENT UPDATE: Empty Start Mode + GUI Synchronization

### What Changed:
- System now starts with **0 plants** and **0 workers**
- GUI and terminal properly synchronize via InventoryManager singleton
- Plant states reflect in real-time
- Workers can be added dynamically
- Plants can be added dynamically

**See FIXES_APPLIED.txt for complete details**

---

## Quick Start

### Compile
```bash
make
```

### Run Options

**Option 1: GTK+ Desktop GUI**
```bash
./greenhouse
# Select option 1
```

**Option 2: Terminal Simulation (Starts Empty)**
```bash
./greenhouse
# Select option 2 - Will start with 0 plants
# Add plants via NetBeans GUI!
```

**Option 3: Command Mode (for NetBeans GUI)**
```bash
./greenhouse
# Select option 3 - Used by Java GUI
```

### NetBeans GUI Setup
1. Open NetBeans IDE
2. File → Open Project
3. Navigate to: `NetBeansGUI/`
4. Right-click project → Run
5. Use GUI to add plants and workers!

---

## Features

### 5-Tab GUI Interface:
1. **Plant Monitor** - View plants, add new plants, advance lifecycle
2. **Worker Management** - View workers, hire new workers
3. **Customer Orders** - Shopping cart system
4. **Dashboard** - Real-time statistics
5. **Live Feed** - Activity log with timestamps

### Key Functionality:
- ✅ Dynamic plant creation
- ✅ Dynamic worker hiring
- ✅ Lifecycle advancement
- ✅ Order processing
- ✅ Real-time data synchronization
- ✅ Empty initial state

---

## Documentation

| File | Description |
|------|-------------|
| **FIXES_APPLIED.txt** | Summary of all fixes applied |
| **FRESH_START_GUIDE.txt** | How to use empty start mode |
| **ENHANCED_GUI_GUIDE.txt** | Complete GUI feature guide |
| **NETBEANS_STEP_BY_STEP.txt** | NetBeans setup instructions |

---

## Testing

Quick test checklist:
- [ ] Compile with `make`
- [ ] Run terminal (option 2) - shows 0 plants
- [ ] Run NetBeans GUI - tables empty
- [ ] Add plant via GUI - appears in table
- [ ] Add worker via GUI - appears in table
- [ ] Refresh - data persists
- [ ] Advance lifecycle - state changes
- [ ] Create order - processes successfully

---

## Clean Build
```bash
make clean
make
```

## Authors
COS 214 Project Team

## License
Academic Project - University of Pretoria
