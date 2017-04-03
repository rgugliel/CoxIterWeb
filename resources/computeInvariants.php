<?php
header('Content-type: text/xml; Charset=utf-8');
echo '<?xml version="1.0" encoding="UTF-8" ?><doc>';

function main()
{
	$graph = isset($_POST['graph']) ? trim($_POST['graph']) : '';
	
	if (empty($graph))
		return '<error>graph empty</error>';

	if (!preg_match("/^[[:digit:]\[\],;]+$/", $graph))
		return '<error>graph format</error>';

	$pieces = explode(';', $graph);
	$piecesCount = count($pieces);
	for ($i = 0; $i < $piecesCount; $i++)
	{
		if (!preg_match("/^\[[[:digit:]]{1,4}(,\[[[:digit:]]{1,4},[[:digit:]]{1,4}\])+\]$/", $pieces[$i]))
                	return '<error>graph format</error>';
	}
}

echo main();

echo '</doc>';
?>
