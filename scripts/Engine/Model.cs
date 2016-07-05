using System;
using System.Text;

namespace BH
{
    public class BHModel
    {
        public string Model = "";
        private int AnimationController = -1;

        public BHModel( string model )
        {
            Model = model;
        }

        public void InitialiseModel()
        {
            AnimationController = CPlusPlusInterface.CreateAnimationController( Model );
        }

        public void ShutdownModel()
        {
            CPlusPlusInterface.DeleteAnimationController( AnimationController );
        }
    }
}
