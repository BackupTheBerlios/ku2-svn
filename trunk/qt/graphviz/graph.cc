// Self-include:
#include "graph.hh"

using namespace ku2::graphviz;

Graph::Graph(const QString &name):
		m_name(name)
{
	// Default graph attributes:
	setMeta("overlap", "prism");
	setMeta("splines", "true");
	setMeta("pad", "0,2");
	setMeta("dpi", "96,0");
	setMeta("nodesep", "0,4");


//
//	    //Set default attributes for the future nodes
//	    _agnodeattr(_graph, "fixedsize", "true");
//	    _agnodeattr(_graph, "label", "");
//	    _agnodeattr(_graph, "regular", "true");
//
//	    //Divide the wanted width by the DPI to get the value in points
//	    QString nodePtsWidth("%1").arg(node_size/_agget(_graph, "dpi", "96,0").toDouble());
//	    //GV uses , instead of . for the separator in floats
//	    _agnodeattr(_graph, "width", nodePtsWidth.replace('.', ","));

}

