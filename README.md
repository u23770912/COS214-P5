# 3 13 14

## TABLE OF CONTENTS
1. [About the Project](#about-the-project)
2. [Team](#team)
3. [Documentation](#documentation)
4. [Setup](#setup)

## About the Project
**Xadrian’s Haven** is a Plant Nursery Simulator developed by Group **3 13 14**. In this immersive simulation, players manage and grow their own nursery, making key decisions to ensure healthy plant growth, customer satisfaction, and sustainable resource use. This project explores the intricacies of plant cultivation, inventory management, sales, and environmental factors affecting plant life. By applying object-oriented principles and design patterns, the simulator provides a scalable and maintainable system that highlights challenges such as limited resources and plant diseases. It delivers an engaging experience that demonstrates how players interact with various nursery elements to build and sustain their thriving Xadrian’s Haven.

## Team
|  | Name | Role|
|-----------|-----------|-----------|
|<img width="200" height="200" alt="Gemini_Generated_Image_4c0a494c0a494c0a" src="https://github.com/user-attachments/assets/8171a34d-d031-4fa0-8509-0130603880dd" />|Luyanda Ndlovu| Row 1, Col 3 |
|<img width="200" height="200" alt="Gemini_Generated_Image_e43r5ke43r5ke43r" src="https://github.com/user-attachments/assets/219bf248-0ebc-49e9-bddd-e247d7b8c93d" />|Warona Moleboge| Row 2, Col 3 |
|<img width="200" height="200" alt="Gemini_Generated_Image_gfk876gfk876gfk8" src="https://github.com/user-attachments/assets/4edfe3dd-0db3-4f81-bd7b-a2a26bd358c3" />|Lerato Matsile| Row 3, Col 3 |
|<img width="200" height="200" alt="Gemini_Generated_Image_gmdbemgmdbemgmdb" src="https://github.com/user-attachments/assets/e8abe2f6-b729-4a61-9e08-83903945ede9" />|Siyabonga Sibiya| Row 4, Col 3 |
|<img width="200" height="200" alt="Gemini_Generated_Image_7cxpab7cxpab7cxp" src="https://github.com/user-attachments/assets/0663ea80-508c-4cf1-9b9e-be4e31274fae" />|Njabulo Nhlengethwa| Row 5, Col 3 |
|<img width="200" height="200" alt="Gemini_Generated_Image_br0v3xbr0v3xbr0v" src="https://github.com/user-attachments/assets/8c939782-5170-414c-a0e5-8da1439e0dd7" />|Tadi Chigeza| Row 6, Col 3 |
|<img width="200" height="200" alt="Gemini_Generated_Image_f5vf31f5vf31f5vf" src="https://github.com/user-attachments/assets/27b254bf-b398-4dcb-9ef7-99e017b3b99a" />|Thabo Seripe| Row 7, Col 3 |


## Documentation
[Report](Documentation/Report.pdf)

[Functional Requirements](Documentation/Functional_and_Non-Functional_Requirements_2.pdf)

[Doxygen Documentation](Documentation/Doxygen/)

[UML Diagrams](UMLS/)

# Setup

### Install Dependencies
sudo apt update

sudo apt install -y build-essential g++ make

sudo apt install -y libgtk-3-dev pkg-config

sudo apt install -y default-jdk

### Building
```bash
# Navigate to project root
cd /path/to/COS214/Prac5

# Clean previous builds (optional)
make clean

# Compile the C++ backend
make
```bash
# Navigate to GUI source directory
cd NetBeansGUI/src

# Compile Java files
javac greenhouse/*.java

# Verify compilation (no output = success)
# Check for .class files
ls greenhouse/*.class
```
```bash
# Navigate to GUI source directory
cd NetBeansGUI/src

# Compile Java files
javac greenhouse/*.java

# Verify compilation (no output = success)
# Check for .class files
ls greenhouse/*.class
```
