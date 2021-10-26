#include "ThreatEvaluater.h"

int main(int argc, char *argv[])
{
	Board board{argv[1]};
	ThreatEvaluator::evaluate(board);
}
