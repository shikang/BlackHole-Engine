using System;
using System.Collections.Generic;

namespace Test
{
  public class TestClass
  {
    public void TestMethod()
    {
      BH.CPlusPlusInterface.StartProfiler("TestMethod");
      Console.WriteLine(mTestString);
      BH.CPlusPlusInterface.StopProfiler("TestMethod");
    }
    
    public void Initialise()
    {
      mTestString = "C# Integrated!";
      mTestInt = 10;
      mTestFloat = 20.0f;
      mTestUInt = 30;
      mTestBoolean = false;
      mTestList.Add(40);
      mTestList.Add(50);
    }
    
    public List<int> mTestList = new List<int>{ 35, 36, 37, 38, 39 };
    public string mTestString = "I love games!";
    public int mTestInt = 1;
    public float mTestFloat = 2.0f;
    public uint mTestUInt = 3;
    public bool mTestBoolean = true;
    
  }
}