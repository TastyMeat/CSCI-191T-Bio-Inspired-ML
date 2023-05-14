clear

load fisheriris.mat

x = meas';
testDimensionsX = 2:10;
%testDimensionsY = 2:10;
%testDimensionsX = zeros(length(testDimensionsY), 1)' + 3;
testDimensionsY = zeros(length(testDimensionsX), 1)' + 3;

results = zeros(length(testDimensionsX), 1);
meanElementPerNode = zeros(length(testDimensionsX), 1);
nets = cell(length(testDimensionsX), 1);

for testId = 1 : length(testDimensionsX)
    %% Create a Self-Organizing Map and Train the Network
    net = selforgmap([testDimensionsX(testId) testDimensionsY(testId)]);
    [net, ~] = train(net,x);
    nets{testId} = net;

    y = net(x);
    classes = vec2ind(y)';

    %% Test the Network
    nodeLabels = cell(testDimensionsX(testId) * testDimensionsY(testId), 1);
    elementsInClasses = zeros(testDimensionsX(testId) * testDimensionsY(testId), 1);
    for i = 1 : testDimensionsX(testId) * testDimensionsY(testId)
        elementIdsInClass = find(classes == i);
        elementsInClasses(i) = length(elementIdsInClass);
        if isempty(elementIdsInClass)
            continue;
        end

        % Mode for cellstr
        nodeUniques = unique(species(elementIdsInClass));
        frequencies = zeros(length(nodeUniques), 1);
        for j = 1 : length(nodeUniques)
            frequencies(j) = length(strcmp(species(elementIdsInClass), nodeUniques{j}));
        end
        [~, modeId] = max(frequencies);
        nodeLabels{i} = nodeUniques{modeId};
    end

    %% Record the Test Results
    results(testId) = sum(strcmp(nodeLabels(classes), species)) / length(species);
    meanElementPerNode(testId) = mean(elementsInClasses);
end
clear testId i j
clear frequencies modeId

%% Display Results
figure("Name", 'Performance of SOM on Iris Data Set');

accuracyPlot = plot(testDimensionsX .* testDimensionsY, results, 'xb-');
hold on
ElementPerNodePlot = plot(testDimensionsX .* testDimensionsY, meanElementPerNode / length(species), 'xr-');
hold off
title('Performance of SOM on Iris Data Set');
xlabel('Number of nodes');
legend({'Accuracy', 'ElementPerNodeRatio'}, "Location", "east");

% Plots
%figure, plotsomtop(net)
%figure, plotsomnc(net)
%figure, plotsomnd(net)
%figure, plotsomplanes(net)
%figure, plotsomhits(net,x)
%figure, plotsompos(net,x)