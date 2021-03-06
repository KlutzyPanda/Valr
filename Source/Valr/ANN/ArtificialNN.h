// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkLayer.h"
/**
 * 
 */
using namespace::std;

class  ArtificialNN
{
public:
	class Replay
	{
	public:
		TArray<double> States;	//Save this
		double Reward = 0;

		Replay(double s1, double s2, double s3, double s4, double s5, double s6, double s7, float r)
		{
			States.Add(s1);
			States.Add(s2);
			States.Add(s3);
			States.Add(s4);
			States.Add(s5);
			States.Add(s6);
			States.Add(s7);
			Reward = r;
		}
	};
public:
	int numInputs;
	int numOutputs;
	int numHidden;
	int numNPerHidden;
	double alpha;
	
	TArray<NetworkLayer *> layers;

	ArtificialNN(int nIn, int nOut, int nHidden, int nPH, double a);
	~ArtificialNN();

	TArray<double> Train(TArray<double> inputValues, TArray<double> desiredOutput);

	TArray<double> CalcOutput(TArray<double> inputValues);

	TArray<double> CalcOutput(TArray<double> inputValues, TArray<double> desiredOutput);

	FString PrintWeights();

	void UpdateWeights(TArray<double> outputs, TArray<double> desiredOutput);

	void LoadWeights(const char* filePath);

	void SaveWeights(FString fileName);

	double ActivationFunction(double value);

	double ActivationFunctionO(double value);

	double Step(double value);

	double TanH(double value);

	double Sigmoid(double value);

	double ReLu(double value);

	double Linear(double value);

	double LeakyReLu(double value);

};

/**
	public int numInputs;
	public int numOutputs;
	public int numHidden;
	public int numNPerHidden;
	public double alpha;
	List<Layer> layers = new List<Layer>();

	public ANN(int nI, int nO, int nH, int nPH, double a)
	{
		numInputs = nI;
		numOutputs = nO;
		numHidden = nH;
		numNPerHidden = nPH;
		alpha = a;

		if(numHidden > 0)
		{
			layers.Add(new Layer(numNPerHidden, numInputs));

			for(int i = 0; i < numHidden-1; i++)
			{
				layers.Add(new Layer(numNPerHidden, numNPerHidden));
			}

			layers.Add(new Layer(numOutputs, numNPerHidden));
		}
		else
		{
			layers.Add(new Layer(numOutputs, numInputs));
		}
	}

	public List<double> Train(List<double> inputValues, List<double> desiredOutput)
	{
		List<double> outputValues = new List<double>();
		outputValues = CalcOutput(inputValues, desiredOutput);
		UpdateWeights(outputValues, desiredOutput);
		return outputValues;
	}

	public List<double> CalcOutput(List<double> inputValues, List<double> desiredOutput)
	{
		List<double> inputs = new List<double>();
		List<double> outputValues = new List<double>();
		int currentInput = 0;

		if(inputValues.Count != numInputs)
		{
			Debug.Log("ERROR: Number of Inputs must be " + numInputs);
			return outputValues;
		}

		inputs = new List<double>(inputValues);
		for(int i = 0; i < numHidden + 1; i++)
		{
				if(i > 0)
				{
					inputs = new List<double>(outputValues);
				}
				outputValues.Clear();

				for(int j = 0; j < layers[i].numNeurons; j++)
				{
					double N = 0;
					layers[i].neurons[j].inputs.Clear();

					for(int k = 0; k < layers[i].neurons[j].numInputs; k++)
					{
						layers[i].neurons[j].inputs.Add(inputs[currentInput]);
						N += layers[i].neurons[j].weights[k] * inputs[currentInput];
						currentInput++;
					}

					N -= layers[i].neurons[j].bias;

					if(i == numHidden)
						layers[i].neurons[j].output = ActivationFunctionO(N);
					else
						layers[i].neurons[j].output = ActivationFunction(N);

					outputValues.Add(layers[i].neurons[j].output);
					currentInput = 0;
				}
		}
		return outputValues;
	}

	public string PrintWeights()
	{
		string weightStr = "";
		foreach(Layer l in layers)
		{
			foreach(Neuron n in l.neurons)
			{
				foreach(double w in n.weights)
				{
					weightStr += w + ",";
				}
			}
		}
		return weightStr;
	}

	public void LoadWeights(string weightStr)
	{
		if(weightStr == "") return;
		string[] weightValues = weightStr.Split(',');
		int w = 0;
		foreach(Layer l in layers)
		{
			foreach(Neuron n in l.neurons)
			{
				for(int i = 0; i < n.weights.Count; i++)
				{
					n.weights[i] = System.Convert.ToDouble(weightValues[w]);
					w++;
				}
			}
		}
	}

	void UpdateWeights(List<double> outputs, List<double> desiredOutput)
	{
		double error;
		for(int i = numHidden; i >= 0; i--)
		{
			for(int j = 0; j < layers[i].numNeurons; j++)
			{
				if(i == numHidden)
				{
					error = desiredOutput[j] - outputs[j];
					layers[i].neurons[j].errorGradient = outputs[j] * (1-outputs[j]) * error;
				}
				else
				{
					layers[i].neurons[j].errorGradient = layers[i].neurons[j].output * (1-layers[i].neurons[j].output);
					double errorGradSum = 0;
					for(int p = 0; p < layers[i+1].numNeurons; p++)
					{
						errorGradSum += layers[i+1].neurons[p].errorGradient * layers[i+1].neurons[p].weights[j];
					}
					layers[i].neurons[j].errorGradient *= errorGradSum;
				}
				for(int k = 0; k < layers[i].neurons[j].numInputs; k++)
				{
					if(i == numHidden)
					{
						error = desiredOutput[j] - outputs[j];
						layers[i].neurons[j].weights[k] += alpha * layers[i].neurons[j].inputs[k] * error;
					}
					else
					{
						layers[i].neurons[j].weights[k] += alpha * layers[i].neurons[j].inputs[k] * layers[i].neurons[j].errorGradient;
					}
				}
				layers[i].neurons[j].bias += alpha * -1 * layers[i].neurons[j].errorGradient;
			}

		}

	}

	double ActivationFunction(double value)
	{
		return TanH(value);
	}

	double ActivationFunctionO(double value)
	{
		return TanH(value);
	}

	double TanH(double value)
	{
		double k = (double) System.Math.Exp(-2*value);
		return 2 / (1.0f + k) - 1;
	}

	double Sigmoid(double value)
	{
		double k = (double) System.Math.Exp(value);
		return k / (1.0f + k);
	}


**/