# Customer Order Test - Interactive Mode

## Overview
This test simulates the complete customer ordering process with cashier validation in **interactive mode**. The customer can build an order through the console, and then a cashier validates the order against available inventory.

## Features Demonstrated

### Design Patterns
- **Builder Pattern**: ConcreteOrderBuilder builds complex orders
- **Composite Pattern**: SinglePlant (leaf) and PlantBundle (composite) 
- **Chain of Responsibility**: Multiple cashiers handle validation
- **Singleton Pattern**: InventoryManager manages sales floor inventory
- **State Pattern**: Plants transition to ReadyForSaleState before being sold
- **Observer Pattern**: (not used in this test but available)

### Workflow
1. **Customer Registration**: Enter name, email, phone
2. **Order Building**: Interactive menu to:
   - View available plants
   - Add single plants to order
   - Create plant bundles with discount
   - View current order
3. **Order Submission**: Submit order for validation
4. **Cashier Validation**: OrderValidationHandler checks inventory availability
5. **Result**: Success or failure with detailed feedback

## How to Run

### Compile the Test
```bash
make test_customer_order
```

### Run the Test
```bash
./test_customer_order
```

## Test Flow Example

```
╔════════════════════════════════════════╗
║   GREENHOUSE CUSTOMER ORDER SYSTEM    ║
╚════════════════════════════════════════╝

=== Setting up Sales Floor Inventory ===
Inventory populated with 5 plants
=========================================

=== Setting up Sales Staff ===
Sales team configured with 2 cashiers
==================================

=== Customer Information ===
Enter your name: John Doe
Enter your email: john@example.com
Enter your phone: 555-1234

Welcome, John Doe!
===========================

╔════════════════════════════════════════╗
║     CUSTOMER ORDER MENU               ║
╠════════════════════════════════════════╣
║ 1. View Available Plants              ║
║ 2. Add Single Plant to Order          ║
║ 3. Add Plant Bundle to Order          ║
║ 4. View Current Order                 ║
║ 5. Submit Order for Validation        ║
║ 6. Exit                               ║
╚════════════════════════════════════════╝
Enter your choice: 
```

## Menu Options

### 1. View Available Plants
Shows all plants currently on the sales floor with their species names.

### 2. Add Single Plant to Order
- Select a plant by number
- Specify quantity
- Plant is added to your order at R25.99 each

### 3. Add Plant Bundle to Order
- Create a custom bundle
- Set bundle name and discount percentage (0-50%)
- Add multiple plants to the bundle
- Discount is applied to the total bundle price

### 4. View Current Order
Displays formatted order summary with:
- Order ID
- Customer name
- Order date
- All items with descriptions
- Total amount

### 5. Submit Order for Validation
- Submits order to cashier validation chain
- **OrderValidationHandler** checks:
  - Order is not empty
  - All plants are available in sales floor inventory
  - Sufficient quantity for each plant type
- Results:
  - ✅ **SUCCESS**: Order validated, ready for payment
  - ❌ **FAILED**: Insufficient inventory, error details shown

### 6. Exit
Exits the application

## Sample Session

```
Enter your choice: 2
Enter plant number to add (1-5): 1
Enter quantity: 2
✓ Added 2x Aloe Vera to order

Enter your choice: 4
╔════════════════════════════════════════╗
║           ORDER SUMMARY               ║
╠════════════════════════════════════════╣
║ Order ID: ORD-1730123456-1            ║
║ Customer: John Doe                     ║
║ Date: 2025-10-28 15:45:00             ║
║ Status: Pending                        ║
╠════════════════════════════════════════╣
║ ITEMS:                                ║
║ 1. 2x Aloe Vera (medium)              ║
║    R51.98                             ║
╠════════════════════════════════════════╣
║ TOTAL: R51.98                         ║
╚════════════════════════════════════════╝

Enter your choice: 5

╔════════════════════════════════════════╗
║   SUBMITTING ORDER FOR VALIDATION     ║
╚════════════════════════════════════════╝

=== Cashier Validation Process ===
A cashier will now validate your order...

--- Order Validation ---
[Order Validation] Checking inventory availability...
[Order Validation] Available plants: 5
[Order Validation] [OK] Aloe Vera: 2 required, 2 available
[Order Validation] All items are available in inventory
[SUCCESS] Order Validation completed successfully

✅ ORDER VALIDATED SUCCESSFULLY!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Order Status: Validated
Total Amount: R51.98

Your order is ready for payment processing!
```

## Validation Rules

The cashier validates:
1. **Order not empty**: At least one item must be in the order
2. **Plant availability**: Each plant type must have sufficient stock
3. **Bundle validation**: All items in bundles are checked individually

## Error Handling

### Insufficient Inventory
```
[ERROR] Insufficient Aloe Vera: 3 required, only 2 available

❌ ORDER VALIDATION FAILED!
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Please review the errors above and modify your order.
```

### Empty Order
```
⚠ Cannot submit empty order!
```

## Files Created

- `CustomerOrderTest.cpp` - Main interactive test
- `Order.cpp` - Order class implementation
- `OrderItem.cpp` - Base class for order items
- `SinglePlant.cpp` - Single plant implementation
- `PlantBundle.cpp` - Plant bundle implementation
- `ConcreteOrderBuilder.cpp` - Order builder implementation
- `PlaceOrderCommand.cpp` - Command for placing orders

## Next Steps

After successful validation, the order would typically proceed to:
1. **Payment Processing** (PaymentProcessHandler)
2. **Inventory Update** (Remove sold items)
3. **Notification** (NotificationHandler sends confirmation)

These handlers can be added to the Chain of Responsibility for complete order processing.
