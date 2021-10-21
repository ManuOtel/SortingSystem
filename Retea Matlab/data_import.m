%%% Citim Datele %%%
clear all
rawData = readtable('emnist-letters-train.csv');
labels_train = rawData(:,1);
labels_train=table2array(labels_train);
imgs_train_2d = rawData(:,2:785);
imgs_train_2d=table2array(imgs_train_2d);
imgs_train = [;;];
len = size(rawData);
len = len(1);
for i=1:len
    imgs_train(:,:,i) = reshape(imgs_train_2d(i,:),28,28);
end
imgs_train = reshape(imgs_train,[28,28,1,len]);
clear imgs_train_2d
clear len
clear rawData
clear i

rawData = readtable('emnist-letters-test.csv');
labels_test = rawData(:,1);
labels_test=table2array(labels_test);
imgs_test_2d = rawData(:,2:785);
imgs_test_2d=table2array(imgs_test_2d);
imgs_test = [;;];
len = size(rawData);
len = len(1);
for i=1:len
    imgs_test(:,:,i) = reshape(imgs_test_2d(i,:),28,28);
end
imgs_test = reshape(imgs_test,[28,28,1,len]);
clear imgs_test_2d
clear len
clear rawData
clear i

labels_train = categorical(labels_train);
labels_test = categorical(labels_test);