# Iris Petal Length Prediction: Simple Neural Network

This project demonstrates the process of predicting the petal length of the Iris-Setosa class from the Iris dataset using a simple neural network. The project utilizes **scikit-learn** for data handling and **PyTorch** for building and training the neural network. This project continues the study of simple neural networks, applying them to a real-world dataset to predict continuous values.

## Overview

The goal of this project is to predict the petal length of the Iris-Setosa class using features such as sepal length, sepal width, and petal width. The project includes data preprocessing, neural network creation, model training, and performance evaluation.

## Code Summary

- **Data Acquisition**: The Iris dataset is fetched using `load_iris()` from **scikit-learn**.
- **Data Filtering**: The dataset is filtered to focus only on the **Iris-Setosa** class.
- **Data Splitting**: The dataset is split into training (80%) and testing (20%) sets.
- **Feature Scaling**: The features are standardized using **StandardScaler** to improve the neural network's performance.
- **Neural Network**: A simple feed-forward neural network with one hidden layer is created using **PyTorch**.
- **Training**: The network is trained using the **Mean Squared Error (MSE)** loss function and the **Adam optimizer**.
- **Evaluation**: The model is evaluated on the test set, and the test loss is displayed.

This project showcases the continued exploration and application of neural networks for regression tasks, using **PyTorch** to create, train, and evaluate the model on real-world data.
