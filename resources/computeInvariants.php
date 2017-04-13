<?php
header('Content-type: text/xml; Charset=utf-8');
echo '<?xml version="1.0" encoding="UTF-8" ?><doc>';

function main()
{
	require '../config/config.php';
	$mDigits = $config['verticesMaximalDigits'];
	$mDigitsWeights = $config['maximalWeightDigits'];

	$graph = isset($_POST['graph']) ? trim($_POST['graph']) : '';
	$dimension = isset($_POST['dimension']) ? (int)$_POST['dimension'] : 0;
	$defaultGraph = $graph == $config['defaultGraph'];
	
	if (empty($graph))
		return '<error>graph empty</error>';

	if (!preg_match("/^[[:digit:]\[\],;]+$/", $graph))
		return '<error>graph format</error>';
	
	$pieces = explode(';', $graph);
	$piecesCount = count($pieces);
	
	if ($piecesCount > $config['maximalNumberVertices'])
		return '<error>vertices maximal number</error>';

	for ($i = 0; $i < $piecesCount; $i++)
	{
		$matches = array();
		if (!preg_match_all("/^\[[[:digit:]]{1,$mDigits}(,\[[[:digit:]]{1,$mDigits},[[:digit:]]{1,$mDigitsWeights}\])+\]$/", $pieces[$i], $matches))
			return '<error>graph format</error>';
	}

	$bTime = microtime(true);
	exec('../' . $config['relativePathToExecutable'] . '/coxiterweb "' . $graph . ';dimension=' . $dimension . '"', $output);
	$eTime = microtime(true);
	
	// ---------------------------------------
	// Output and logging
	echo implode("\n", $output);
	@file_put_contents('../log/' . date('Y-m') . '.txt', 
		date('d/m/Y H:m:i') . "\t\t" . $piecesCount . "\t\t" . ($eTime - $bTime) . ($defaultGraph ? "\t\tdefault" : '') . "\n", 
		FILE_APPEND);
}

echo main();

echo '</doc>';
?>
