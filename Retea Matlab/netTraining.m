layers =[
    imageInputLayer([28 28 1])
	
    convolution2dLayer(3,16,'Padding',1)
    batchNormalizationLayer
    reluLayer
	
    maxPooling2dLayer(2,'Stride',2)
    
    convolution2dLayer(3,32,'Padding',1)
    batchNormalizationLayer
    reluLayer
	
    maxPooling2dLayer(2,'Stride',2)
	
    convolution2dLayer(3,64,'Padding',1)
    batchNormalizationLayer
    reluLayer
	
    maxPooling2dLayer(2,'Stride',2)
	
    convolution2dLayer(3,128,'Padding',1)
    batchNormalizationLayer
    reluLayer
    
    maxPooling2dLayer(2,'Stride',2)
    
    convolution2dLayer(3,256,'Padding',1)
    batchNormalizationLayer
    reluLayer
	
    fullyConnectedLayer(26)
    softmaxLayer
    classificationLayer
];
miniBatchSize = 8192/2;
options = trainingOptions( 'sgdm',...
    'MiniBatchSize', miniBatchSize,...
    'Plots', 'training-progress');

net = trainNetwork(imgs_train, labels_train', layers, options);

%%% Hai cu verificarea %%%

predLabelsTest = net.classify(imgs_test);
accuracy = sum(predLabelsTest == labels_test) / numel(labels_test)




