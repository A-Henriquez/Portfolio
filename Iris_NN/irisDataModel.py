
'''
model that predicts petal length of iris using the iris data set from sklearn

problem of: 3 classes of iris present: Iris-Setosa - Iris-Versicolour - Iris-Virginica
Each class has a different average petal length thus its a generalization of the 3 classes
 For later: want to filter out for a single class
'''
import pandas as pd
import numpy as np

from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split

from sklearn.preprocessing import StandardScaler


import torch
import torch.nn as nn
import torch.optim as optim



#loads iris data as pandas dataframe
iris_data = load_iris(as_frame=True)

print("names of dataset columns ", iris_data.feature_names)
print("names of taget classes ",iris_data.target_names)


df = iris_data.frame #due to bunch grouping from the imported iris data; need to be able to process 
#print(df)

#print(df.columns) # print columns to check their names

# filter for a single iris class - Iris-Setosa
filtered_df = df[df['target'] == 0] # 0 is target integer for setosa variant
#print(filtered_df)

# sort it by petal length
#sorted_df = df.sort_values(by="petal length (cm)")


# looking to predict petal length

# split data set
X = filtered_df.drop(columns=["petal length (cm)", "target"]) ## dropping columns petal length as this is the feature we use for predictions and target since we dont need it 
y = filtered_df["petal length (cm)"]

x_train, x_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)


# scale the data
scaler = StandardScaler() #getting the trainer ready
x_train_scaled = scaler.fit_transform(x_train) # Training group gets fit
x_test_scaled = scaler.transform(x_test)  # Test group gets the same treatment


# create neural network

class SimpleNN(nn.Module):

    ## initialization method
        # constructor method that inits the network
        # input_size is the number of input features

    def __init__(self, input_size):
        super(SimpleNN, self).__init__()

        # Defining layers

            # fully connected layer with input_size and 10 outputs
            # linear transformation to the input data
        self.fc1 = nn.Linear(input_size, 10)

            # fully connected layer of 10 inputs and 1 output
            # for final regression output; petal length
        self.fc2 = nn.Linear(10, 1)


    def forward(self, x):

        ## define how data flows through the network input x passed to this method

        # Activation function for the first layer
        # applies first layer to input x
        x = torch.relu(self.fc1(x))  

         # Output layer
         # passes output from first layer to second layer
        x = self.fc2(x)       

        return x



# get number of features
input_size =x_train_scaled.shape[1]

## initalize the model
model = SimpleNN(input_size)


### train the model
#Loss Function and Optimizer

# Mean Squared Error for regression 
# -- measures the difference from what we'd expect from the score and what we got
# averages the squared errors for overall score of how well they are meeting expectations
# the lower the mse the better the perforance aligned with what was expected
criterion = nn.MSELoss()  

# Adam optimizer
# the coach that judges and adjust what needs to be changed
# the loss = performance score
# gradients = ids where improvements are needed
# weights = making the changes to the models parameter to minimize the loss 
# lr = learning rate; how aggresively ( speed and carefulness) the changes should be made
optimizer = optim.Adam(model.parameters(), lr=0.01) 


## training loop
num_epochs = 100
for epoch in range(num_epochs):
    model.train # set to training mode

    # Convert training data to tensors

    # inputs = transforms training input into a format that can be learned from
    inputs = torch.tensor(x_train_scaled, dtype=torch.float32)
    # targets = expected scores that should be acheived after learning from the training input 
    targets = torch.tensor(y_train.values, dtype=torch.float32).view(-1, 1)  # Reshape targets

    # Forward pass
    outputs = model(inputs)  # Get predictions
    loss = criterion(outputs, targets)  # Calculate loss

    # Backward pass and optimization
    optimizer.zero_grad()  # Clear previous gradients
    loss.backward()  # Compute gradients
    optimizer.step()  # Update weights

    if (epoch + 1) % 10 == 0:  # Print loss every 10 epochs
        print(f'Epoch [{epoch + 1}/{num_epochs}], Loss: {loss.item():.4f}')


# evaluate the model on the test portion of the data
# set to evaluate mode
model.eval()

with torch.no_grad(): # stops the gradient calculation -- no feedback or adjustment given
    test_inputs = torch.tensor(x_test_scaled, dtype=torch.float32)
    test_outputs = model(test_inputs)  # Get predictions on test data
    test_loss = criterion(test_outputs, torch.tensor(y_test.values, dtype=torch.float32).view(-1, 1))
    print(f'Test Loss: {test_loss.item():.4f}')

