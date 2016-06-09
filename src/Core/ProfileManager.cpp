// Precompiled Header
#include "Core/Stdafx.h"

#include "Core/ProfileManager.h"
#include "Core/XMLSerialiser.h"

#include "Platform/ClockCycle.h"

#include <fstream>
#include <iomanip>
#include <sstream>

#if BH_PROFILING_ENABLED == BH_ENABLED
namespace BH
{
	s64 ProfileManager::Hz;
	String ProfileManager::ReportDirectory = "";

	ProfileManager::ProfileManager()
		: mRoot(nullptr)
		, mCurrent(nullptr)
		, mTotalCycles(0)
	{
		Hz = CalculateProcessorFrequency();
	}

	//Destructor
	ProfileManager::~ProfileManager()
	{
		// Print info
		//PrintProfileReport();

		// Empty list
		Clear();
	}

	// Start profiler of this name
	void ProfileManager::StartProfiler(const Char * name)
	{
		if (ProfileNode * node = FindCurrentNode(name))
		{
			mCurrent = node;
		}
		else
		{
			mCurrent = AddCurrentNode(name);
		}

		mCurrent->mProfiler.Start();
	}

	// End profiler of this name
	void ProfileManager::EndProfiler(const Char * name)
	{
		// If current node not this function name
		if (mCurrent->mFunctionName != name)
			return;

		mCurrent->mProfiler.Stop();
		mCurrent = mCurrent->mParent;
	}

	// Find Node by name
	ProfileManager::ProfileNode * ProfileManager::FindCurrentNode(const Char * name)
	{
		// no profiler running
		if (!mCurrent)
		{
			ProfileNode * current = mRoot;
			while (current)
			{
				if (current->mFunctionName == name)
					return current;

				current = current->mSiblings;
			}

			return nullptr;
		}

		// If current node is (recursion)
		if (mCurrent->mFunctionName == name)
			return mCurrent;

		// Check for current nodes childrens
		ProfileNode * current = mCurrent->mChildren;
		while (current)
		{
			if (current->mFunctionName == name)
				return current;

			current = current->mSiblings;
		}

		// Cannot be found
		return nullptr;
	}

	// Add Current Node
	ProfileManager::ProfileNode * ProfileManager::AddCurrentNode(const Char * name)
	{
		ProfileNode * node = new ProfileNode();
		node->mFunctionName = name;
		node->mStrFunctionName = name;
		node->mParent = mCurrent;

		// List empty
		if (!mRoot)
		{
			mRoot = node;
		}
		else
		{
			// Check for all current node siblings
			ProfileNode ** current = (!mCurrent) ? &mRoot : &mCurrent->mChildren;
			while (*current)
			{
				current = &(*current)->mSiblings;
			}

			*current = node;
		}

		return node;

	}

	SerialiseElement * ProfileManager::AddTable(SerialiseElement * elem, SerialiseDoc & doc)
	{
		SerialiseElement * table_elem = nullptr;
		if (elem)
		{
			SerialiseElement * insert_elem = XMLSerialiser::PushElem("tr", elem, doc);
			insert_elem = XMLSerialiser::PushElem("td", insert_elem, doc);
			XMLSerialiser::SetAttribute("colspan", 6, insert_elem);
			table_elem = XMLSerialiser::PushElem("Table", insert_elem, doc);
		}
		else
			table_elem = XMLSerialiser::PushElem("Table", doc);

		XMLSerialiser::SetAttribute("border", 0, table_elem);
		XMLSerialiser::SetAttribute("style", "width:99%", table_elem);
		XMLSerialiser::SetAttribute("align", "right", table_elem);

		return table_elem;
	}

	void ProfileManager::AddHeader(SerialiseElement * table_elem, SerialiseDoc & doc)
	{
		SerialiseElement * row_elem = XMLSerialiser::PushElem("tr", table_elem, doc);
		XMLSerialiser::SetAttribute("bgcolor", "#3366FF", row_elem);

		// Header
		SerialiseElement * header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "Section");

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "Seconds");

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "Calls");

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "Total Cycles");

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "%");

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "%Total");
	}

	void ProfileManager::AddElement(SerialiseElement * table_elem, SerialiseDoc & doc, ProfileNode * node, s64 total_cycles)
	{
		SerialiseElement * row_elem = XMLSerialiser::PushElem("tr", table_elem, doc);

		// Header
		SerialiseElement * value_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", value_elem);
		XMLSerialiser::SetText(value_elem, node->mStrFunctionName.c_str());

		value_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", value_elem);
		XMLSerialiser::SetText(value_elem, static_cast<f64>( node->mProfiler.GetTicksTaken() ) / Hz);

		value_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", value_elem);
		XMLSerialiser::SetText(value_elem, node->mProfiler.GetSamples());

		value_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", value_elem);
		XMLSerialiser::SetText( value_elem, static_cast<u32>( node->mProfiler.GetTicksTaken() ) );

		std::stringstream stream;
		stream << std::setprecision(3) << static_cast<f64>( node->mProfiler.GetTicksTaken() ) * 100.0 / static_cast<f64>(total_cycles) << "%";
		value_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", value_elem);
		XMLSerialiser::SetText(value_elem, stream.str().c_str());

		stream.str("");
		stream << std::setprecision(3) << static_cast<f64>( node->mProfiler.GetTicksTaken() ) * 100.0 / static_cast<f64>(mTotalCycles) << "%";
		value_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", value_elem);
		XMLSerialiser::SetText(value_elem, stream.str().c_str());
	}

	void ProfileManager::AddFooter(SerialiseElement * table_elem, SerialiseDoc & doc, s64 parent_total, s64 child_total, u32 samples)
	{
		SerialiseElement * row_elem = XMLSerialiser::PushElem("tr", table_elem, doc);
		XMLSerialiser::SetAttribute("bgcolor", "#808080", row_elem);

		// Unprofiled
		s64 cycles_left = parent_total - child_total;

		SerialiseElement * header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "Unprofiled");

		header_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, static_cast<f64>(cycles_left) / Hz);

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, samples);

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, static_cast<u32>(cycles_left));

		std::stringstream stream;
		stream << std::setprecision(3) << static_cast<f64>(cycles_left)* 100.0 / static_cast<f64>(parent_total) << "%";
		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, stream.str().c_str());

		stream.str("");
		stream << std::setprecision(3) << static_cast<f64>(cycles_left)* 100.0 / static_cast<f64>(mTotalCycles) << "%";
		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, stream.str().c_str());

		// Footer
		row_elem = XMLSerialiser::PushElem("tr", table_elem, doc);
		XMLSerialiser::SetAttribute("bgcolor", "#FF6666", row_elem);

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "Total");

		header_elem = XMLSerialiser::PushElem("td", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, static_cast<f64>(parent_total) / Hz);

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, samples);

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, static_cast<u32>(parent_total));

		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, "100%");

		stream.str("");
		stream << std::setprecision(3) << static_cast<f64>(parent_total)* 100.0 / static_cast<f64>(mTotalCycles) << "%";
		header_elem = XMLSerialiser::PushElem("th", row_elem, doc);
		XMLSerialiser::SetAttribute("align", "left", header_elem);
		XMLSerialiser::SetText(header_elem, stream.str().c_str());
	}

	// Report time taken for each function profiled
	void ProfileManager::PrintProfileReport()
	{
		SerialiseDoc doc;
		SerialiseElement * table_elem = AddTable(nullptr, doc);
		AddHeader(table_elem, doc);

		ProfileNode * current = mRoot;
		while (current)
		{
			mTotalCycles += current->mProfiler.GetTicksTaken();
			current = current->mSiblings;
		}

		PrintNode(doc, nullptr, table_elem, mRoot, mTotalCycles, true);

		XMLSerialiser::SaveFile(ReportDirectory + "Profiler.html", doc);
	}

	void ProfileManager::PrintNode(SerialiseDoc & doc,
		SerialiseElement * parent_elem,
		SerialiseElement * table_elem,
		ProfileNode * node,
		s64 total_cycles,
		bool child)
	{
		AddElement(table_elem, doc, node, total_cycles);

		if (node->mChildren)
		{
			SerialiseElement * sub_table_elem = AddTable(table_elem, doc);
			AddHeader(sub_table_elem, doc);

			//file << std::setfill('-') << std::setw(140) << std::endl;
			PrintNode(doc, table_elem, sub_table_elem, node->mChildren, node->mProfiler.GetTicksTaken(), true);
		}

		if (node->mSiblings)
		{
			PrintNode(doc, parent_elem, table_elem, node->mSiblings, total_cycles, false);
		}

		if (child)
		{
			s64 child_total = node->mProfiler.GetTicksTaken();

			while (node->mSiblings)
			{
				child_total += node->mSiblings->mProfiler.GetTicksTaken();
				node = node->mSiblings;
			}

			AddFooter(table_elem, doc, total_cycles, child_total, node->mProfiler.GetSamples());
		}
	}

	// Clear all profiler
	void ProfileManager::Clear()
	{
		if (mRoot)
			DeleteNode(mRoot);

		mRoot = nullptr;
		mCurrent = nullptr;
	}

	void ProfileManager::DeleteNode(ProfileNode * node)
	{
		if (node->mChildren)
			DeleteNode(node->mChildren);

		if (node->mSiblings)
			DeleteNode(node->mSiblings);

		delete node;
	}
}
#endif