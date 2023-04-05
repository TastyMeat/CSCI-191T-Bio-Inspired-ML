load fisheriris.mat

x = meas';
testDimensions = 2:10;
results = zeros(length(testDimensions), 1);
meanElementPerCluster = zeros(length(testDimensions), 1);

for testId = 1 : length(testDimensions)
    %% Create a Self-Organizing Map
    net = selforgmap([testDimensions(testId) testDimensions(testId)]);

    %% Train the Network
    [net, ~] = train(net,x);

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
        [~, id] = max(frequencies);
        clusterLabels{i} = clusterUniques{id};
    end

    results(testId) = sum(strcmp(clusterLabels(classes), species)) / length(species);
    meanElementPerCluster(testId) = mean(elementsInClasses);
end

figure
hold on
plot(power(testDimensions, 2), results, 'b');
plot(power(testDimensions, 2), meanElementPerCluster / length(species), 'r');
hold off

figure, plot(power(testDimensions, 2), meanElementPerCluster ./ power(testDimensions, 2)', 'r');


% Plots
% Uncomment these lines to enable various plots.
%figure, plotsomtop(net)
%figure, plotsomnc(net)
%figure, plotsomnd(net)
%figure, plotsomplanes(net)
%figure, plotsomhits(net,x)
%figure, plotsompos(net,x)