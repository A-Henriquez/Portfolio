# California Housing Project: Simple Neural Network

This project demonstrates the process of preprocessing the California Housing dataset, training a simple neural network to predict housing values, and evaluating the model's performance. The project utilizes **scikit-learn** for data handling and **PyTorch** for building and training the neural network. It serves as a practice activity to try out creating a simple neural network for the first time.

## Overview

The goal of this project is to predict the median house value based on features such as average room size, population, and income. The project covers key tasks like data preprocessing, neural network creation, model training, and performance evaluation.

## Code Summary

- **Data Acquisition**: The California housing dataset is fetched using `fetch_california_housing()` from **scikit-learn**.
- **Data Cleaning**: Houses with an average room size larger than 50 are filtered out to improve model performance.
- **Data Splitting**: The dataset is split into training (80%) and testing (20%) sets.
- **Feature Scaling**: Features are standardized using **StandardScaler** to enhance the performance of the neural network.
- **Neural Network**: A simple feed-forward neural network with one hidden layer is created using **PyTorch**.
- **Training**: The network is trained using the **Mean Squared Error (MSE)** loss function and the **Adam optimizer**.
- **Evaluation**: The model is evaluated on the test set, and the test loss is displayed.
- **Predictions**: The trained model makes predictions on the test set, with the first 10 predicted values displayed.
