<?php
header('Content-type: text/xml; Charset=utf-8');
echo '<?xml version="1.0" encoding="UTF-8" ?><doc>';

function main()
{
	$graph = isset($_POST['graph']) ? $_POST['graph'] : '';
	
	if (empty($graph))
		return '<error>graph empty</error>';
}

echo main();

echo '</doc>';
?>
