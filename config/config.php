<?php
$config['maximalNumberVertices'] = 20;
$config['maximalWeight'] = 255;
$config['updateGraphTimeout'] = 1000;
$config['relativePathToExecutable'] = 'cpp/build/';

// -----------------
// DO NOT EDIT BELOW
$config['verticesMaximalDigits'] = ceil(log($config['maximalNumberVertices'],10));
$config['maximalWeightDigits'] = ceil(log($config['maximalWeight'],10));
$config['defaultGraph'] = '[0,[1,3]];[1,[2,3]];[2,[3,3],[4,4]]';
?>
