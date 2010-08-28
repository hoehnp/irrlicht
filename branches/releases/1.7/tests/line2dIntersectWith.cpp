// Copyright (C) 2008-2009 Colin MacDonald
// No rights reserved: this software is in the public domain.

#include "testUtils.h"

using namespace irr;
using namespace core;

static bool testLines(line2df const & line1,
					  line2df const & line2,
					  bool expectedHit,
					  const vector2df & expectedIntersection)
{
	bool gotExpectedResult = true;

	logTestString("\nLine 1 = %.1f %.1f to %.1f %.1f \n",
		line1.start.X, line1.start.Y,
		line1.end.X, line1.end.Y);
	logTestString("Line 2 = %.1f %.1f to %.1f %.1f\n",
		line2.start.X, line2.start.Y,
		line2.end.X, line2.end.Y);

	vector2df intersection;
	logTestString("line1 with line2 = ");
	if(line1.intersectWith(line2, intersection))
	{
		logTestString("hit at %.1f %.1f - ",
			intersection.X, intersection.Y);

		if(!line1.isPointOnLine(intersection) || !line2.isPointOnLine(intersection))
		{
			logTestString("ERROR! point is not on both lines - ");
			gotExpectedResult = false;
		}

		if(expectedHit)
		{
			if(intersection == expectedIntersection)
			{
				logTestString("expected\n");
			}
			else
			{
				logTestString("unexpected intersection (expected %.1f %.1f)\n",
					expectedIntersection.X, expectedIntersection.Y);
				gotExpectedResult = false;
			}
		}
		else
		{
			logTestString("UNEXPECTED\n");
			gotExpectedResult = false;
		}
	}
	else
	{
		logTestString("miss - ");
		if(!expectedHit)
		{
			logTestString("expected\n");
		}
		else
		{
			logTestString("UNEXPECTED\n");
			gotExpectedResult = false;
		}
	}

	logTestString("line2 with line1 = ");
	if(line2.intersectWith(line1, intersection))
	{
		logTestString("hit at %.1f %.1f - ",
			intersection.X, intersection.Y);
		if(!line1.isPointOnLine(intersection) || !line2.isPointOnLine(intersection))
		{
			logTestString("ERROR! point is not on both lines - ");
			gotExpectedResult = false;
		}

		if(expectedHit)
		{
			if(intersection == expectedIntersection)
			{
				logTestString("expected\n");
			}
			else
			{
				logTestString("unexpected intersection (expected %.1f %.1f)\n",
					expectedIntersection.X, expectedIntersection.Y);
				gotExpectedResult = false;
			}
		}
		else
		{
			logTestString("UNEXPECTED\n");
			gotExpectedResult = false;
		}
	}
	else
	{
		logTestString("miss - ");
		if(!expectedHit)
		{
			logTestString("expected\n");
		}
		else
		{
			logTestString("UNEXPECTED\n");
			gotExpectedResult = false;
		}
	}

	return gotExpectedResult;
}

// Test the functionality of line2d>T>::intersectWith().
/** Validation is done with asserts() against expected results. */
bool line2dIntersectWith(void)
{
	bool allExpected = true;

	// Crossing lines, horizontal and vertical
	allExpected &= testLines(line2df(vector2df(1,1),vector2df(1,3)),
							line2df(vector2df(0,2),vector2df(2,2)),
							true, vector2df(1,2));
	assert(allExpected);

	// Crossing lines, both diagonal
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(2,2)),
							line2df(vector2df(0,2),vector2df(2,0)),
							true, vector2df(1,1));
	assert(allExpected);

	// Non-crossing lines, horizontal and vertical
	allExpected &= testLines(line2df(vector2df(1,1),vector2df(1,3)),
							line2df(vector2df(0,4),vector2df(2,4)),
							false, vector2df());
	assert(allExpected);

	// Non-crossing lines, both diagonal
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(2,2)),
							line2df(vector2df(3,4),vector2df(4,3)),
							false, vector2df());
	assert(allExpected);

	// Meeting at a common point
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(1,0),vector2df(2,0)),
							true, vector2df(1,0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(1,0),vector2df(0,1)),
							true, vector2df(1,0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(1,0),vector2df(0,-1)),
							true, vector2df(1,0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(0,1)),
							line2df(vector2df(0,1),vector2df(1,1)),
							true, vector2df(0,1));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(0,1)),
							line2df(vector2df(0,1),vector2df(1,-1)),
							true, vector2df(0,1));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(0,1)),
							line2df(vector2df(0,1),vector2df(0,2)),
							true, vector2df(0,1));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(1,0),vector2df(2,0)),
							true, vector2df(1,0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,1)),
							line2df(vector2df(1,1),vector2df(0,2)),
							true, vector2df(1,1));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,1)),
							line2df(vector2df(1,1),vector2df(2,0)),
							true, vector2df(1,1));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,1)),
							line2df(vector2df(1,1),vector2df(2,2)),
							true, vector2df(1,1));
	assert(allExpected);


	// Parallel lines, no intersection
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(0,1),vector2df(1,1)),
							false, vector2df());
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(0,1)),
							line2df(vector2df(1,0),vector2df(1,1)),
							false, vector2df());
	assert(allExpected);

	// Non parallel lines, no intersection
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(0,1),vector2df(0,2)),
							false, vector2df());
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(0,1)),
							line2df(vector2df(1,0),vector2df(2,0)),
							false, vector2df());
	assert(allExpected);

	// Coincident (and thus parallel) lines
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(0,0),vector2df(1,0)),
							true, vector2df(0,0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(2,0),vector2df(0,2)),
							line2df(vector2df(2,0),vector2df(0,2)),
							true, vector2df(2,0));
	assert(allExpected);

	// Two segments of the same unlimited line, but no intersection
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,1)),
							line2df(vector2df(2,2),vector2df(3,3)),
							false, vector2df());
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(1,0)),
							line2df(vector2df(2,0),vector2df(3,0)),
							false, vector2df());
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(0,1)),
							line2df(vector2df(0,2),vector2df(0,3)),
							false, vector2df());
	assert(allExpected);

	// Overlapping parallel lines
	allExpected &= testLines(line2df(vector2df(1,0),vector2df(2,0)),
							line2df(vector2df(0,0),vector2df(3,0)),
							true, vector2df(1.5f, 0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,1),vector2df(0,2)),
							line2df(vector2df(0,0),vector2df(0,3)),
							true, vector2df(0, 1.5f));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(1,0),vector2df(2,0)),
							line2df(vector2df(0,0),vector2df(3,0)),
							true, vector2df(1.5f, 0));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,1),vector2df(0,2)),
							line2df(vector2df(0,0),vector2df(0,3)),
							true, vector2df(0, 1.5f));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(1,1),vector2df(2,2)),
							line2df(vector2df(0,0),vector2df(3,3)),
							true, vector2df(1.5f, 1.5f));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(1,2),vector2df(2,1)),
							line2df(vector2df(0,3),vector2df(3,0)),
							true, vector2df(1.5f, 1.5f));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(10,8)),
							line2df(vector2df(2.5f,2.0f),vector2df(5.0f,4.0f)),
							true, vector2df(3.75f, 3.0f));
	assert(allExpected);
	allExpected &= testLines(line2df(vector2df(0,0),vector2df(2000,1000)),
							line2df(vector2df(2,1),vector2df(2.2f,1.4f)),
							true, vector2df(2.0f, 1.0f));
	assert(allExpected);

	if(allExpected)
		logTestString("\nAll tests passed\n");
	else
		logTestString("\nFAIL!\n");

	return allExpected;
}

