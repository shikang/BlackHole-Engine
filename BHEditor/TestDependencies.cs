using System;
using System.Collections.Generic;

namespace Test
{
  public class TestDependencies
  {
    public void TestDependenciesMethod( TestClass c )
    {
      c.TestMethod();
    }
    
    public void Update( float dt )
    {
      BH.CPlusPlusInterface.StartProfiler( "Update" );
      TestDependenciesMethod( new TestClass() );
      BH.CPlusPlusInterface.StartProfiler( "Update" );
    }
    
    //public TestClass mTestClass = new TestClass();
    public List<TestClass> mTestComplexList = new List<TestClass>{ new TestClass(), new TestClass() };
    
  }
}