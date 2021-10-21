% Arhitectura retelei neuronale
layers =[
    imageInputLayer([28 28 1])
	
    convolution2dLayer(5,32,'Padding',1)
    reluLayer
    
    convolution2dLayer(5,32,'Padding',1)
    reluLayer
	
    batchNormalizationLayer
    maxPooling2dLayer(2,'Stride',2)
    
    convolution2dLayer(3,128,'Padding',1)
    reluLayer
    
    convolution2dLayer(3,128,'Padding',1)
    reluLayer
    
    batchNormalizationLayer
    maxPooling2dLayer(2,'Stride',2)
    
    convolution2dLayer(3,256)
    reluLayer
    
    convolution2dLayer(3,256)
    reluLayer
    
    batchNormalizationLayer
    maxPooling2dLayer(2,'Stride',2)
	
    fullyConnectedLayer(64)
    
    fullyConnectedLayer(26)
    softmaxLayer
    classificationLayer
];

% Antrenarea retelei neuronale
miniBatchSize = 8192/2;
options = trainingOptions('sgdm',...
    'MiniBatchSize', miniBatchSize,...
    'MaxEpochs',20, ...
    'Plots', 'training-progress');

net = trainNetwork(imgs_train, labels_train', layers, options);

%%% Hai cu verificarea %%%

predLabelsTest = net.classify(imgs_test);
accuracy = sum(predLabelsTest == labels_test) / numel(labels_test)




