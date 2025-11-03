package greenhouse;

import javax.swing.*;
import javax.swing.table.*;
import java.awt.*;
import java.awt.event.*;
import java.util.List;

/**
 * Simple Swing GUI example for NetBeans
 * Shows how to use GreenhouseAPI to communicate with C++ backend
 */
public class MainGUI extends JFrame {
    private GreenhouseAPI api;
    private JTable plantTable;
    private DefaultTableModel plantTableModel;
    private JTable workerTable;
    private DefaultTableModel workerTableModel;
    private JLabel statsLabel;
    private JTextArea liveFeedArea;
    private DefaultListModel<String> cartModel;
    private JList<String> cartList;
    private java.util.Map<String, String> cartPlantIds; // Maps display string to plant ID
    
    public MainGUI() {
        super("Greenhouse Management System - Option B");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(800, 600);
        
        // Initialize API connection to network server
        try {
            // Connect to the C++ server (must be running first!)
            // Start the C++ program with: ./greenhouse -> Select option 2
            String host = "localhost";
            int port = 8765;
            
            System.out.println("Connecting to greenhouse server at " + host + ":" + port + "...");
            api = new GreenhouseAPI(host, port);
            
            System.out.println("Connected to greenhouse server at " + host + ":" + port);
            
            // Initialize cart tracking
            cartPlantIds = new java.util.HashMap<>();
            
            initComponents();
            System.out.println("GUI components initialized");
            
            // Don't load data initially - just show empty GUI
            // User can click refresh to load data
            System.out.println("Skipping initial data load");
            
            addToFeed("Successfully connected to greenhouse server!");
            addToFeed("Server is running in real-time mode");
            addToFeed("Click 'Refresh' buttons to load plants and workers");
            
            System.out.println("GUI ready - window should be visible");
        } catch (Exception e) {
            System.err.println("ERROR: " + e.getMessage());
            e.printStackTrace();
            JOptionPane.showMessageDialog(this, 
                "Failed to connect to greenhouse server at localhost:8765\n\n" +
                "Make sure the C++ server is running first:\n" +
                "1. Run: ./greenhouse\n" +
                "2. Select option: 2 (Integrated Mode)\n" +
                "3. Then start this GUI\n\n" +
                "Error: " + e.getMessage(),
                "Connection Error", 
                JOptionPane.ERROR_MESSAGE);
            System.exit(1);
        }
    }
    
    private void initComponents() {
        JTabbedPane tabbedPane = new JTabbedPane();
        
        // Tab 1: Plant Monitor
        JPanel monitorPanel = createMonitorPanel();
        tabbedPane.addTab("Plant Monitor", monitorPanel);
        
        // Tab 2: Worker Management
        JPanel workerPanel = createWorkerPanel();
        tabbedPane.addTab("Worker Management", workerPanel);
        
        // Tab 3: Customer Orders
        JPanel orderPanel = createOrderPanel();
        tabbedPane.addTab("Customer Orders", orderPanel);
        
        // Tab 4: Dashboard
        JPanel dashboardPanel = createDashboardPanel();
        tabbedPane.addTab("Dashboard", dashboardPanel);
        
        // Tab 5: Live Feed
        JPanel feedPanel = createLiveFeedPanel();
        tabbedPane.addTab("Live Feed", feedPanel);
        
        add(tabbedPane);
    }
    
    private JPanel createMonitorPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        // Table
        String[] columns = {"ID", "Name", "Type", "State", "Price"};
        plantTableModel = new DefaultTableModel(columns, 0);
        plantTable = new JTable(plantTableModel);
        JScrollPane scrollPane = new JScrollPane(plantTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        
        // Buttons
        JPanel buttonPanel = new JPanel();
        JButton refreshButton = new JButton("Refresh");
        refreshButton.addActionListener(e -> loadData());
        
        JButton addButton = new JButton("Add Plant");
        addButton.addActionListener(e -> showAddPlantDialog());
        
        JButton advanceButton = new JButton("Advance Lifecycle");
        advanceButton.addActionListener(e -> advanceSelected());
        
        buttonPanel.add(refreshButton);
        buttonPanel.add(addButton);
        buttonPanel.add(advanceButton);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        return panel;
    }
    
    private JPanel createWorkerPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        // Table
        String[] columns = {"ID", "Name", "Role"};
        workerTableModel = new DefaultTableModel(columns, 0);
        workerTable = new JTable(workerTableModel);
        JScrollPane scrollPane = new JScrollPane(workerTable);
        panel.add(scrollPane, BorderLayout.CENTER);
        
        // Buttons
        JPanel buttonPanel = new JPanel();
        JButton refreshButton = new JButton("Refresh");
        refreshButton.addActionListener(e -> loadWorkers());
        
        JButton addButton = new JButton("Add Worker");
        addButton.addActionListener(e -> showAddWorkerDialog());
        
        buttonPanel.add(refreshButton);
        buttonPanel.add(addButton);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        return panel;
    }
    
    private JPanel createOrderPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        // Top Panel: Bouquet Suggestions
        JPanel bouquetPanel = new JPanel(new BorderLayout());
        bouquetPanel.setBorder(BorderFactory.createTitledBorder("Bouquet Suggestions"));
        
        JPanel bouquetControlPanel = new JPanel();
        JLabel eventLabel = new JLabel("Event Type:");
        String[] eventTypes = {"Birthday", "Wedding", "Graduation", "Funeral"};
        JComboBox<String> eventCombo = new JComboBox<>(eventTypes);
        JButton suggestButton = new JButton("Get Suggestions");
        
        JTextArea suggestionsArea = new JTextArea(8, 40);
        suggestionsArea.setEditable(false);
        suggestionsArea.setFont(new Font("Monospaced", Font.PLAIN, 11));
        JScrollPane suggestionsScroll = new JScrollPane(suggestionsArea);
        
        suggestButton.addActionListener(e -> {
            String eventType = (String) eventCombo.getSelectedItem();
            try {
                String suggestions = api.getSuggestedBouquets(eventType);
                suggestionsArea.setText(suggestions);
                addToFeed("Generated " + eventType + " bouquet suggestions");
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, 
                    "Error getting suggestions: " + ex.getMessage());
                addToFeed("ERROR: Failed to get bouquet suggestions");
            }
        });
        
        bouquetControlPanel.add(eventLabel);
        bouquetControlPanel.add(eventCombo);
        bouquetControlPanel.add(suggestButton);
        bouquetPanel.add(bouquetControlPanel, BorderLayout.NORTH);
        bouquetPanel.add(suggestionsScroll, BorderLayout.CENTER);
        
        // Middle Panel: Available Inventory Section
        JPanel inventoryPanel = new JPanel(new BorderLayout());
        inventoryPanel.setBorder(BorderFactory.createTitledBorder("Available Plants (Sales Floor)"));
        
        String[] inventoryColumns = {"Plant ID", "Species", "State", "Price"};
        DefaultTableModel inventoryTableModel = new DefaultTableModel(inventoryColumns, 0);
        JTable inventoryTable = new JTable(inventoryTableModel);
        inventoryPanel.add(new JScrollPane(inventoryTable), BorderLayout.CENTER);
        
        JPanel inventoryButtonPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JButton refreshInventoryButton = new JButton("Refresh Inventory");
        refreshInventoryButton.addActionListener(e -> {
            try {
                List<GreenhouseAPI.Plant> inventory = api.getInventory();
                inventoryTableModel.setRowCount(0);
                for (GreenhouseAPI.Plant plant : inventory) {
                    inventoryTableModel.addRow(new Object[]{
                        plant.name,
                        plant.type,
                        plant.state,
                        String.format("$%.2f", plant.price)
                    });
                }
                addToFeed("Loaded " + inventory.size() + " plants from sales floor");
            } catch (Exception ex) {
                JOptionPane.showMessageDialog(this, 
                    "Error loading inventory: " + ex.getMessage());
                addToFeed("ERROR: Failed to load inventory");
            }
        });
        
        // Add pot selection dropdown
        JLabel potLabel = new JLabel("  Pot:");
        String[] potOptions = {"Clay", "Plastic", "Wood", "Glass", "Metal"};
        JComboBox<String> potCombo = new JComboBox<>(potOptions);
        potCombo.setSelectedItem("Plastic"); // Default selection
        
        JButton addToCartButton = new JButton("Add to Cart");
        addToCartButton.addActionListener(e -> {
            int selectedRow = inventoryTable.getSelectedRow();
            if (selectedRow >= 0) {
                String plantId = (String) inventoryTableModel.getValueAt(selectedRow, 0);
                String species = (String) inventoryTableModel.getValueAt(selectedRow, 1);
                String price = (String) inventoryTableModel.getValueAt(selectedRow, 3);
                String selectedPot = (String) potCombo.getSelectedItem();
                
                // Add to cart with tracking and pot information
                String displayText = species + " (" + selectedPot + " pot) - " + price + " (ID: " + plantId + ")";
                cartModel.addElement(displayText);
                cartPlantIds.put(displayText, plantId);
                
                // Remove from inventory table (reserved)
                inventoryTableModel.removeRow(selectedRow);
                
                addToFeed("Added to cart: " + species + " in " + selectedPot + " pot (reserved)");
            } else {
                JOptionPane.showMessageDialog(this, 
                    "Please select a plant from inventory first");
            }
        });
        
        inventoryButtonPanel.add(refreshInventoryButton);
        inventoryButtonPanel.add(potLabel);
        inventoryButtonPanel.add(potCombo);
        inventoryButtonPanel.add(addToCartButton);
        inventoryPanel.add(inventoryButtonPanel, BorderLayout.SOUTH);
        
        // Bottom Panel: Shopping cart
        JPanel cartPanel = new JPanel(new BorderLayout());
        cartPanel.setBorder(BorderFactory.createTitledBorder("Shopping Cart"));
        cartModel = new DefaultListModel<>();
        cartList = new JList<>(cartModel);
        cartPanel.add(new JScrollPane(cartList), BorderLayout.CENTER);
        
        JPanel cartButtons = new JPanel();
        JButton removeButton = new JButton("Remove");
        removeButton.addActionListener(e -> removeFromCart());
        
        JButton checkoutButton = new JButton("Checkout");
        checkoutButton.addActionListener(e -> checkout());
        
        cartButtons.add(removeButton);
        cartButtons.add(checkoutButton);
        cartPanel.add(cartButtons, BorderLayout.SOUTH);
        
        // Layout: Top (Bouquet Suggestions), Middle (Inventory), Bottom (Cart)
        JSplitPane middleSplit = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, inventoryPanel, cartPanel);
        middleSplit.setDividerLocation(400);
        
        JSplitPane mainSplit = new JSplitPane(JSplitPane.VERTICAL_SPLIT, bouquetPanel, middleSplit);
        mainSplit.setDividerLocation(200);
        panel.add(mainSplit, BorderLayout.CENTER);
        
        return panel;
    }
    
    private JPanel createLiveFeedPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        liveFeedArea = new JTextArea();
        liveFeedArea.setEditable(false);
        liveFeedArea.setFont(new Font("Monospaced", Font.PLAIN, 12));
        JScrollPane scrollPane = new JScrollPane(liveFeedArea);
        panel.add(scrollPane, BorderLayout.CENTER);
        
        JButton clearButton = new JButton("Clear Feed");
        clearButton.addActionListener(e -> liveFeedArea.setText(""));
        panel.add(clearButton, BorderLayout.SOUTH);
        
        addToFeed("System initialized - Connected to C++ backend");
        
        return panel;
    }
    
    private JPanel createDashboardPanel() {
        JPanel panel = new JPanel(new BorderLayout());
        
        statsLabel = new JLabel("Loading stats...");
        statsLabel.setFont(new Font("Monospaced", Font.PLAIN, 14));
        statsLabel.setBorder(BorderFactory.createEmptyBorder(20, 20, 20, 20));
        panel.add(statsLabel, BorderLayout.CENTER);
        
        JPanel buttonPanel = new JPanel();
        JButton refreshButton = new JButton("Refresh Stats");
        refreshButton.addActionListener(e -> loadStats());
        
        JButton printStructureButton = new JButton("Print Structure to Terminal");
        printStructureButton.setToolTipText("Prints the greenhouse structure and all plants to the C++ terminal");
        printStructureButton.addActionListener(e -> printStructure());
        
        JButton exitButton = new JButton("Exit System");
        exitButton.setToolTipText("Shutdown the greenhouse system gracefully");
        exitButton.addActionListener(e -> shutdownSystem());
        
        buttonPanel.add(refreshButton);
        buttonPanel.add(printStructureButton);
        buttonPanel.add(exitButton);
        panel.add(buttonPanel, BorderLayout.SOUTH);
        
        return panel;
    }
    
    private void loadData() {
        try {
            List<GreenhouseAPI.Plant> plants = api.getAllPlants();
            plantTableModel.setRowCount(0); // Clear table
            
            for (GreenhouseAPI.Plant plant : plants) {
                plantTableModel.addRow(new Object[]{
                    plant.id,
                    plant.name,
                    plant.type,
                    plant.state,
                    String.format("$%.2f", plant.price)
                });
            }
            addToFeed("Loaded " + plants.size() + " plants");
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "Error loading plants: " + e.getMessage());
            addToFeed("ERROR: Failed to load plants");
        }
    }
    
    private void loadWorkers() {
        try {
            List<GreenhouseAPI.Worker> workers = api.getWorkers();
            workerTableModel.setRowCount(0); // Clear table
            
            for (GreenhouseAPI.Worker worker : workers) {
                workerTableModel.addRow(new Object[]{
                    worker.id,
                    worker.name,
                    worker.role
                });
            }
            addToFeed("Loaded " + workers.size() + " workers");
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "Error loading workers: " + e.getMessage());
            addToFeed("ERROR: Failed to load workers");
        }
    }
    
    private void loadStats() {
        try {
            GreenhouseAPI.Stats stats = api.getStats();
            String text = String.format(
                "<html><h2>Greenhouse Statistics</h2>" +
                "<table>" +
                "<tr><td><b>Total Plants:</b></td><td>%d</td></tr>" +
                "<tr><td><b>Healthy Plants:</b></td><td>%d</td></tr>" +
                "<tr><td><b>Total Orders:</b></td><td>%d</td></tr>" +
                "<tr><td><b>Revenue:</b></td><td>$%.2f</td></tr>" +
                "</table></html>",
                stats.totalPlants,
                stats.healthyPlants,
                stats.totalOrders,
                stats.revenue
            );
            statsLabel.setText(text);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "Error loading stats: " + e.getMessage());
        }
    }
    
    private void advanceSelected() {
        int selectedRow = plantTable.getSelectedRow();
        if (selectedRow >= 0) {
            int plantId = (int) plantTableModel.getValueAt(selectedRow, 0);
            try {
                api.advanceLifecycle(plantId);
                addToFeed("Advanced lifecycle for plant " + plantId);
                loadData(); // Refresh to show new state
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, 
                    "Error: " + e.getMessage());
                addToFeed("ERROR: Failed to advance plant " + plantId);
            }
        } else {
            JOptionPane.showMessageDialog(this, 
                "Please select a plant first");
        }
    }
    
    private void showAddPlantDialog() {
        // Create dropdown menus with proper categorization
        String[] flowerSpecies = {"Rose", "Tulip", "Orchid", "Lily", "Sunflower", "Lavender"};
        String[] treeSpecies = {"Bonsai", "Oak", "Maple", "Pine", "Willow"};
        String[] succulentSpecies = {"Aloe", "Cactus", "Echeveria", "Jade", "Succulent"};
        
        String[] allSpecies = {"Rose", "Tulip", "Orchid", "Lily", "Sunflower", "Lavender",
                               "Bonsai", "Oak", "Maple", "Pine", "Willow",
                               "Aloe", "Cactus", "Echeveria", "Jade", "Succulent"};
        
        JComboBox<String> speciesCombo = new JComboBox<>(allSpecies);
        JLabel typeLabel = new JLabel("Type: Flower");
        JTextField priceField = new JTextField(10);
        priceField.setText("25.00");
        
        // Update type label when species changes
        speciesCombo.addActionListener(e -> {
            String species = (String) speciesCombo.getSelectedItem();
            String type = getPlantType(species, flowerSpecies, treeSpecies, succulentSpecies);
            typeLabel.setText("Type: " + type);
        });
        
        JPanel panel = new JPanel(new GridLayout(3, 2, 5, 5));
        panel.add(new JLabel("Species:"));
        panel.add(speciesCombo);
        panel.add(new JLabel("Type:"));
        panel.add(typeLabel);
        panel.add(new JLabel("Price:"));
        panel.add(priceField);
        
        int result = JOptionPane.showConfirmDialog(this, panel, 
            "Add New Plant", JOptionPane.OK_CANCEL_OPTION);
        
        if (result == JOptionPane.OK_OPTION) {
            try {
                String species = (String) speciesCombo.getSelectedItem();
                String type = getPlantType(species, flowerSpecies, treeSpecies, succulentSpecies);
                String pot = "Plastic"; // Default pot for greenhouse
                double price = Double.parseDouble(priceField.getText());
                
                String plantId = api.addPlant(species, type, pot, price);
                addToFeed("Created new plant: " + plantId + " (" + species + " - " + type + ")");
                loadData(); // Refresh table
                JOptionPane.showMessageDialog(this, 
                    "Plant created successfully: " + plantId);
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, 
                    "Error creating plant: " + e.getMessage());
                addToFeed("ERROR: Failed to create plant");
            }
        }
    }
    
    private String getPlantType(String species, String[] flowers, String[] trees, String[] succulents) {
        for (String f : flowers) if (f.equals(species)) return "Flower";
        for (String t : trees) if (t.equals(species)) return "Tree";
        for (String s : succulents) if (s.equals(species)) return "Succulent";
        return "Flower"; // Default
    }
    
    private void showAddWorkerDialog() {
        JTextField nameField = new JTextField(15);
        
        // Create dropdown for worker roles
        String[] roleOptions = {"Gardener", "Cashier"};
        JComboBox<String> roleCombo = new JComboBox<>(roleOptions);
        
        JPanel panel = new JPanel(new GridLayout(2, 2, 5, 5));
        panel.add(new JLabel("Name:"));
        panel.add(nameField);
        panel.add(new JLabel("Role:"));
        panel.add(roleCombo);
        
        int result = JOptionPane.showConfirmDialog(this, panel, 
            "Add New Worker", JOptionPane.OK_CANCEL_OPTION);
        
        if (result == JOptionPane.OK_OPTION) {
            try {
                String name = nameField.getText();
                String role = (String) roleCombo.getSelectedItem();
                
                if (name.trim().isEmpty()) {
                    JOptionPane.showMessageDialog(this, "Name cannot be empty!");
                    return;
                }
                
                String response = api.addWorker(name, role);
                addToFeed("Added worker: " + name + " (" + role + ")");
                loadWorkers(); // Refresh table
                JOptionPane.showMessageDialog(this, response);
            } catch (Exception e) {
                JOptionPane.showMessageDialog(this, 
                    "Error adding worker: " + e.getMessage());
                addToFeed("ERROR: Failed to add worker");
            }
        }
    }
    
    private void addToCart() {
        int selectedRow = plantTable.getSelectedRow();
        if (selectedRow >= 0) {
            int plantId = (int) plantTableModel.getValueAt(selectedRow, 0);
            String plantName = (String) plantTableModel.getValueAt(selectedRow, 1);
            String price = (String) plantTableModel.getValueAt(selectedRow, 4);
            
            String displayText = plantName + " - " + price + " (ID: " + plantId + ")";
            cartModel.addElement(displayText);
            cartPlantIds.put(displayText, String.valueOf(plantId));
            
            addToFeed("Added to cart: " + plantName);
        } else {
            JOptionPane.showMessageDialog(this, 
                "Please select a plant first");
        }
    }
    
    private void removeFromCart() {
        int selectedIndex = cartList.getSelectedIndex();
        if (selectedIndex >= 0) {
            String item = cartModel.get(selectedIndex);
            cartModel.remove(selectedIndex);
            cartPlantIds.remove(item);
            addToFeed("Removed from cart: " + item + " (released back to inventory)");
        }
    }
    
    private void checkout() {
        if (cartModel.isEmpty()) {
            JOptionPane.showMessageDialog(this, 
                "Cart is empty!");
            return;
        }
        
        try {
            // Extract plant IDs from cart using the tracking map
            java.util.List<String> plantIds = new java.util.ArrayList<>();
            for (int i = 0; i < cartModel.size(); i++) {
                String displayText = cartModel.get(i);
                String plantId = cartPlantIds.get(displayText);
                if (plantId != null) {
                    plantIds.add(plantId);
                }
            }
            
            String response = api.createOrder(plantIds);
            addToFeed("Order created: " + response);
            JOptionPane.showMessageDialog(this, response);
            
            // Clear cart and tracking
            cartModel.clear();
            cartPlantIds.clear();
            
            // Refresh data
            loadData();
            loadStats();
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "Error creating order: " + e.getMessage());
            addToFeed("ERROR: Failed to create order - " + e.getMessage());
        }
    }
    
    private void printStructure() {
        try {
            api.printStructure();
            addToFeed("Greenhouse structure printed to C++ terminal");
            JOptionPane.showMessageDialog(this, 
                "Greenhouse structure has been printed to the C++ terminal.\n" +
                "Check the terminal window where the C++ program is running.",
                "Structure Printed", 
                JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, 
                "Error printing structure: " + e.getMessage());
            addToFeed("ERROR: Failed to print structure");
        }
    }
    
    private void shutdownSystem() {
        int confirm = JOptionPane.showConfirmDialog(this,
            "Are you sure you want to shutdown the greenhouse system?\n" +
            "This will close both the GUI and the C++ backend.",
            "Confirm Shutdown",
            JOptionPane.YES_NO_OPTION,
            JOptionPane.WARNING_MESSAGE);
        
        if (confirm == JOptionPane.YES_OPTION) {
            try {
                addToFeed("Shutting down greenhouse system...");
                api.close(); // Sends EXIT command to backend
                addToFeed("Backend shutdown complete");
                System.exit(0);
            } catch (Exception e) {
                addToFeed("ERROR: " + e.getMessage());
                System.exit(1);
            }
        }
    }
    
    private void addToFeed(String message) {
        String timestamp = new java.text.SimpleDateFormat("HH:mm:ss").format(new java.util.Date());
        liveFeedArea.append("[" + timestamp + "] " + message + "\n");
        liveFeedArea.setCaretPosition(liveFeedArea.getDocument().getLength());
    }
    
    public static void main(String[] args) {
        System.out.println("Starting MainGUI...");
        SwingUtilities.invokeLater(() -> {
            System.out.println("Creating GUI instance...");
            MainGUI gui = new MainGUI();
            System.out.println("Setting visible...");
            gui.setVisible(true);
            gui.setLocationRelativeTo(null); // Center on screen
            gui.toFront(); // Bring to front
            gui.requestFocus(); // Request focus
            System.out.println("GUI should now be visible!");
        });
    }
}
