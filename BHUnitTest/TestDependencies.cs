using System;
using System.Collections.Generic;

namespace Test
{
  public class TestDependencies
  {
    public void TestDependenciesMethod( TestClass c )
    {
      Console.WriteLine("-Calling from TestDependencies script");
      c.TestMethod();
      Console.WriteLine("-Finish calling from TestDependencies script");
    }
    
    public TestClass mTestClass = new TestClass();
    public List<TestClass> mTestComplexList = new List<TestClass>{ new TestClass(), new TestClass() };
    
  }
}