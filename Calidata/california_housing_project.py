## california housing project -- preprocessing and importing data
from sklearn.datasets import fetch_california_housing
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler

import pandas as pd

import torch
import torch.nn as nn
import torch.optim as optim



# step 1: Get Data 
## gets housing data
housing = fetch_california_housing()



# step 2: Organize Data
## put in pd datafram
df = pd.DataFrame(housing.data, columns=housing.feature_names)

## sort it by average room size
sortedDF = df.sort_values(by="AveRooms")



# step 3: Clean Data
## filter to get rid of all houses with average room as bigger than 50
filtered = sortedDF[sortedDF["AveRooms"] < 50]



# step 4: Split Data
## split data into 2 set to be able to train the model ; 1 for training and another for testing 
### Assuming you want to predict the target variable 'MedHouseVal'
X = filtered  # Features
y = housing.target[filtered.index]  # Target variable

x_train, x_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)


# step 5: Scale Data and process data
## making sure all data is on the same scale. 
scaler = StandardScaler()
x_train_scale = scaler.fit_transform(x_train)
x_test_scale = scaler.transform(x_test)

# Convert data to PyTorch tensors
X_train_tensor = torch.FloatTensor(x_train_scale)
y_train_tensor = torch.FloatTensor(y_train).view(-1, 1)  # Reshape for the model
X_test_tensor = torch.FloatTensor(x_test_scale)
y_test_tensor = torch.FloatTensor(y_test).view(-1, 1)



# Step 6: Create the Neural Network
class SimpleNN(nn.Module):
    def __init__(self):
        super(SimpleNN, self).__init__()
        self.fc1 = nn.Linear(X_train_tensor.shape[1], 64)  # Input layer
        self.fc2 = nn.Linear(64, 32)                        # Hidden layer
        self.fc3 = nn.Linear(32, 1)                         # Output layer

    def forward(self, x):
        x = torch.relu(self.fc1(x))  # Activation function for the first layer
        x = torch.relu(self.fc2(x))  # Activation function for the second layer
        x = self.fc3(x)               # Output layer
        return x

# Initialize the model
model = SimpleNN()



# step 7: Train model
## training
# Set the loss function and optimizer
criterion = nn.MSELoss()  # Mean Squared Error loss
optimizer = optim.Adam(model.parameters(), lr=0.001)

# Training loop
epochs = 100
for epoch in range(epochs):
    model.train()
    
    # Zero the gradients
    optimizer.zero_grad()
    
    # Forward pass
    outputs = model(X_train_tensor)
    
    # Compute loss
    loss = criterion(outputs, y_train_tensor)
    
    # Backward pass and optimization
    loss.backward()
    optimizer.step()
    
    if (epoch + 1) % 10 == 0:
        print(f'Epoch [{epoch + 1}/{epochs}], Loss: {loss.item():.4f}')



# step 8: evaluate
model.eval()  # Set the model to evaluation mode
with torch.no_grad():
    test_outputs = model(X_test_tensor)
    test_loss = criterion(test_outputs, y_test_tensor)

print(f'Test Loss: {test_loss.item():.4f}')



# step 9: make a prediction
with torch.no_grad():
    predictions = model(X_test_tensor)
    print("Predictions: ", predictions[:10])  # Display first 10 predictions



