# CoxIterWeb
A small web client for CoxIter.
Demo: https://coxiter.rafaelguglielmetti.ch

Features:
* Preview of the graph with vis.js (https://github.com/mdaines/viz.js)
* Computation of the invariants (f-vector, Euler characteristic, compactness and cofiniteness, growth series and growth rate)
* Download the invariants and/or the graph

Requirements:
* A web server running on Linux (with PHP and the exec function)
* CoxIter installed

Additional remarks:
* One could/should move the executable outside of the www/ folder (see config/config.php)
