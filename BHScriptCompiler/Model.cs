using System;
using System.Text;

namespace BH
{
    public class BHModel
    {
        public enum ANIMATION
        {
            INVALID = 0,
        };

        public string Model = "";
        private int AnimationController = (int)ANIMATION.INVALID;

        public BHModel( string model )
        {
            Model = model;
        }

        public void InitialiseModel()
        {
            if( AnimationController != 0 )
            {
                ShutdownModel();
            }

            Console.WriteLine("Initialising model: " + Model);
            AnimationController = CPlusPlusInterface.CreateAnimationController( Model );
        }

        public void ShutdownModel()
        {
            CPlusPlusInterface.DeleteAnimationController( AnimationController );
            AnimationController = (int)ANIMATION.INVALID;
        }

        public int GetAnimation()
        {
            return AnimationController;
        }
    }
}
