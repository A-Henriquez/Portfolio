California Housing Project: Preprocessing and Neural Network Model
This project demonstrates how to preprocess the California Housing dataset, train a simple neural network to predict housing values, and evaluate the model's performance. The project utilizes scikit-learn for data handling and PyTorch for building and training the neural network.

Overview
The goal is to predict the median house value based on various features such as average room size, population, and income. The project covers data preprocessing, neural network creation, model training, and evaluation.

Code Summary
Data Acquisition: Fetches the California housing dataset using fetch_california_housing() from scikit-learn.
Data Cleaning: Filters out houses with an average room size larger than 50.
Data Splitting: Splits the dataset into training (80%) and testing (20%) sets.
Feature Scaling: Standardizes the features using StandardScaler for better model performance.
Neural Network: A simple feed-forward neural network is created with one hidden layer.
Training: The network is trained using the Mean Squared Error (MSE) loss function and Adam optimizer.
Evaluation: The model is evaluated on the test set, and the test loss is printed.
Predictions: The model makes predictions on the test set, showing the first 10 predicted values.
