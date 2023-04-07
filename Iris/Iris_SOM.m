clear

load fisheriris.mat

x = meas';
testDimensions = 2:10;
results = zeros(length(testDimensions), 1);
meanElementPerCluster = zeros(length(testDimensions), 1);
nets = cell(length(testDimensions), 1);

for testId = 1 : length(testDimensions)
    %% Create a Self-Organizing Map and Train the Network
    net = selforgmap([testDimensions(testId) testDimensions(testId)]);
    [net, ~] = train(net,x);
    nets{testId} = net;

    y = net(x);
    classes = vec2ind(y)';

    %% Test the Network
    clusterLabels = cell(power(testDimensions(testId), 2), 1);
    elementsInClasses = zeros(power(testDimensions(testId), 2), 1);
    for i = 1 : power(testDimensions(testId), 2)
        elementIdsInClass = find(classes == i);
        elementsInClasses(i) = length(elementIdsInClass);
        if isempty(elementIdsInClass)
            continue;
        end

        % Mode for cellstr
        clusterUniques = unique(species(elementIdsInClass));
        frequencies = zeros(length(clusterUniques), 1);
        for j = 1 : length(clusterUniques)
            frequencies(j) = length(strcmp(species(elementIdsInClass), clusterUniques{j}));
        end
        [~, modeId] = max(frequencies);
        clusterLabels{i} = clusterUniques{modeId};
    end

    %% Record the Test Results
    results(testId) = sum(strcmp(clusterLabels(classes), species)) / length(species);
    meanElementPerCluster(testId) = mean(elementsInClasses);
end
clear testId i j
clear frequencies modeId

%% Display Results
figure("Name", 'Performance of SOM on Iris Data Set');

accuracyPlot = plot(power(testDimensions, 2), results, 'xb-');
hold on
ElementPerClusterPlot = plot(power(testDimensions, 2), meanElementPerCluster / length(species), 'xr-');
hold off
title('Performance of SOM on Iris Data Set');
xlabel('Number of nodes');
legend({'Accuracy', 'ElementPerClusterRatio'}, "Location", "east");

% Plots
%figure, plotsomtop(net)
%figure, plotsomnc(net)
%figure, plotsomnd(net)
%figure, plotsomplanes(net)
%figure, plotsomhits(net,x)
%figure, plotsompos(net,x)