    MRF.correctBoundaryVelocity(U);

    fvVectorMatrix UEqn
    (
        fvm::ddt(rhof, U) + fvm::div(rhof*phi/porosityf, U)
      - fvm::laplacian(muf, U)
//      + MRF.DDt(rho, U)
//      + turbulence->divDevRhoReff(rho, U)
//     ==
//        fvOptions(rho, U)
    );

    #include "UEqnAddPorosity.H"

    UEqn.relax();

    fvOptions.constrain(UEqn);

    if (pimple.momentumPredictor())
    {
        solve
        (
            UEqn
         ==
            fvc::reconstruct
            (
                (
//                    mixture.surfaceTensionForce()
//                  - ghf*fvc::snGrad(rho)
                  - fvc::snGrad(p_rgh)
                ) * mesh.magSf() //Correct with Sf here even with porosity? 
            )
        );

        fvOptions.correct(U);
    }
