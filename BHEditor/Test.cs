using System;
using System.Collections.Generic;

namespace Test
{
  public class TestClass
  {
    public void TestMethod()
    {
      BH.CPlusPlusInterface.StartProfiler("TestMethod");
      BH.CPlusPlusInterface.StopProfiler("TestMethod");
    }
    
    public void Update( float dt )
    {
      Console.WriteLine( "FPS: {0}", dt );
      TestMethod();
    }
    
    public void Initialise()
    {
    }
    
    public List<int> mTestList = new List<int>{ 1, 2 };
    
  }
}