<?php
header('Content-type: text/xml; Charset=utf-8');
echo '<?xml version="1.0" encoding="UTF-8" ?><doc>';

function main()
{
	require '../config/config.php';
	$mDigits = $config['verticesMaximalDigits'];	

	$graph = isset($_POST['graph']) ? trim($_POST['graph']) : '';
	$dimension = isset($_POST['dimension']) ? (int)$_POST['dimension'] : 0;
	
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
		if (!preg_match_all("/^\[[[:digit:]]{1,$mDigits}(,\[[[:digit:]]{1,$mDigits},[[:digit:]]{1,$mDigits}\])+\]$/", $pieces[$i], $matches))
			return '<error>graph format</error>';
	}

	exec('../' . $config['relativePathToExecutable'] . '/coxiterweb "' . $graph . ';dimension=' . $dimension . '"', $output);
	echo implode("\n", $output);
}

echo main();

echo '</doc>';
?>
