package greenhouse;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * Socket-based API to communicate with C++ Greenhouse backend (Option B)
 * Connects to the integrated simulation server on localhost:8765
 */
public class GreenhouseAPI {
    private Socket socket;
    private BufferedWriter writer;
    private BufferedReader reader;
    
    /**
     * Connect to the C++ server (must be running first)
     * @param host Server hostname (usually "localhost")
     * @param port Server port (usually 8765)
     */
    public GreenhouseAPI(String host, int port) throws IOException {
        // Connect to the server
        socket = new Socket(host, port);
        
        // Setup streams
        writer = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
        reader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        
        System.out.println("Connected to greenhouse server at " + host + ":" + port);
    }
    
    public List<Plant> getAllPlants() throws IOException {
        writer.write("GET_PLANTS\n");
        writer.flush();
        
        List<Plant> plants = new ArrayList<>();
        String line = reader.readLine(); // "PLANTS <count>"
        
        while (!(line = reader.readLine()).equals("END")) {
            String[] parts = line.split("\\|");
            if (parts.length == 5) {
                Plant plant = new Plant();
                plant.id = Integer.parseInt(parts[0]);
                plant.name = parts[1];
                plant.type = parts[2];
                plant.state = parts[3];
                plant.price = Double.parseDouble(parts[4]);
                plants.add(plant);
            }
        }
        
        return plants;
    }
    
    public List<Plant> getInventory() throws IOException {
        writer.write("GET_INVENTORY\n");
        writer.flush();
        
        List<Plant> plants = new ArrayList<>();
        String line = reader.readLine(); // "INVENTORY <count>"
        
        while (!(line = reader.readLine()).equals("END")) {
            String[] parts = line.split("\\|");
            if (parts.length == 4) {
                Plant plant = new Plant();
                plant.name = parts[0];
                plant.type = parts[1];
                plant.state = parts[2];
                plant.price = Double.parseDouble(parts[3]);
                plants.add(plant);
            }
        }
        
        return plants;
    }
    
    public Stats getStats() throws IOException {
        writer.write("GET_STATS\n");
        writer.flush();
        
        Stats stats = new Stats();
        String line = reader.readLine(); // "STATS"
        
        while (!(line = reader.readLine()).equals("END")) {
            String[] parts = line.split("\\|");
            if (parts.length == 2) {
                switch (parts[0]) {
                    case "total_plants":
                        stats.totalPlants = Integer.parseInt(parts[1]);
                        break;
                    case "healthy_plants":
                        stats.healthyPlants = Integer.parseInt(parts[1]);
                        break;
                    case "orders":
                        stats.totalOrders = Integer.parseInt(parts[1]);
                        break;
                    case "revenue":
                        stats.revenue = Double.parseDouble(parts[1]);
                        break;
                }
            }
        }
        
        return stats;
    }
    
    public void advanceLifecycle(int plantId) throws IOException {
        writer.write("ADVANCE " + plantId + "\n");
        writer.flush();
        reader.readLine(); // Read OK/ERROR response
    }
    
    public String addPlant(String species, String type, String pot, double price) throws IOException {
        String command = "ADD_PLANT " + species + " " + type + " " + pot + " " + price + "\n";
        System.out.println("Sending: " + command.trim());
        writer.write(command);
        writer.flush();
        System.out.println("Waiting for response...");
        String response = reader.readLine(); // "OK <plantId> created successfully"
        System.out.println("Received: " + response);
        if (response != null && response.startsWith("OK")) {
            String[] parts = response.split(" ");
            if (parts.length > 1) {
                return parts[1]; // Extract plant ID
            }
        }
        throw new IOException("Invalid response from server: " + response);
    }
    
    public List<Worker> getWorkers() throws IOException {
        writer.write("GET_WORKERS\n");
        writer.flush();
        
        List<Worker> workers = new ArrayList<>();
        String line = reader.readLine(); // "WORKERS <count>"
        
        while (!(line = reader.readLine()).equals("END")) {
            String[] parts = line.split("\\|");
            if (parts.length == 3) {
                Worker worker = new Worker();
                worker.id = Integer.parseInt(parts[0]);
                worker.name = parts[1];
                worker.role = parts[2];
                workers.add(worker);
            }
        }
        
        return workers;
    }
    
    public String addWorker(String name, String role) throws IOException {
        writer.write("ADD_WORKER " + name + " " + role + "\n");
        writer.flush();
        String response = reader.readLine(); // "OK Worker <name> added"
        return response;
    }
    
    public String createOrder(List<String> plantIds) throws IOException {
        StringBuilder cmd = new StringBuilder("CREATE_ORDER");
        for (String id : plantIds) {
            cmd.append(" ").append(id);
        }
        writer.write(cmd.toString() + "\n");
        writer.flush();
        String response = reader.readLine(); // "OK Order created. Total: $<amount>"
        return response;
    }
    
    public void printStructure() throws IOException {
        writer.write("PRINT_STRUCTURE\n");
        writer.flush();
        reader.readLine(); // Read OK response
    }
    
    public String getSuggestedBouquets(String eventType) throws IOException {
        writer.write("SUGGEST_BOUQUET " + eventType + "\n");
        writer.flush();
        
        StringBuilder result = new StringBuilder();
        String line = reader.readLine(); // "SUGGESTIONS" or "ERROR"
        
        if (line.startsWith("ERROR")) {
            throw new IOException(line.substring(6)); // Remove "ERROR " prefix
        }
        
        while (!(line = reader.readLine()).equals("END")) {
            result.append(line).append("\n");
        }
        
        return result.toString();
    }
    
    public void close() throws IOException {
        if (socket != null && !socket.isClosed()) {
            writer.write("EXIT\n");
            writer.flush();
            socket.close();
        }
    }
    
    // Data classes
    public static class Plant {
        public int id;
        public String name;
        public String type;
        public String state;
        public double price;
        
        @Override
        public String toString() {
            return String.format("%s (%s) - %s - $%.2f", name, type, state, price);
        }
    }
    
    public static class Stats {
        public int totalPlants;
        public int healthyPlants;
        public int totalOrders;
        public double revenue;
    }
    
    public static class Worker {
        public int id;
        public String name;
        public String role;
        
        @Override
        public String toString() {
            return String.format("%s - %s", name, role);
        }
    }
}

