<?php
header('Content-type: text/xml; Charset=utf-8');
echo '<?xml version="1.0" encoding="UTF-8" ?><doc>';

function main()
{
	require '../config/config.php';
	$mDigits = $config['verticesMaximalDigits'];	

	$graph = isset($_POST['graph']) ? trim($_POST['graph']) : '';
	
	if (empty($graph))
		return '<error>graph empty</error>';

	if (!preg_match("/^[[:digit:]\[\],;]+$/", $graph))
		return '<error>graph format</error>';
	
	// TODO: tester longueur
	$pieces = explode(';', $graph);
	$piecesCount = count($pieces);
	for ($i = 0; $i < $piecesCount; $i++)
	{
		if (!preg_match("/^\[[[:digit:]]{1,4}(,\[[[:digit:]]{1,$mDigits},[[:digit:]]{1,$mDigits}\])+\]$/", $pieces[$i]))
                	return '<error>graph format</error>';
	}

	exec('../' . $config['relativePathToExecutable'] . '/coxiterweb "' . $graph . '"', $output);
	echo implode("\n", $output);
}

echo main();

echo '</doc>';
?>
