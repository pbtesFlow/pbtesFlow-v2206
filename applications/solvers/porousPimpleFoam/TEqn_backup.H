//if (porosityEnabled)
{
//    const volScalarField& porosity = tporosity.cref();

    // NOTE: ENSURE THAT dEfluid + dEsolid = 0!!!!

    // Fluid temperature equation
    Info << "Solving Tf equation" << endl;
    volScalarField dT(Ts-Tf);

    fvScalarMatrix TfEqn
    (
        fvm::ddt(porosity*rhof*cpf,Tf)
        + fvm::div(rhof*cpf*phi, Tf)
        - fvm::laplacian(porosityf*kf, Tf)
        ==
//        fvOptions(Tf)     //NOTE: MESSES UP UNITS!!!
        fvm::Su(porosity*Cf*dT, Tf)
    );

    TfEqn.relax();
    fvOptions.constrain(TfEqn);
    TfEqn.solve();
    fvOptions.correct(Tf);

    // Updating temperature dependent fluid viscosity
    Info << "Updating temperature dependent viscosity" << endl;
    muf = mu + (mu2-mu)*(Tf-Tmu)/(Tmu2-Tmu);

    // Solid temperature equation
    Info << "Solving Ts equation" << endl;

    fvScalarMatrix TsEqn
    (
        fvm::ddt((1-porosity)*rhos*cps, Ts)
        - fvm::laplacian((1-porosityf)*ks, Ts)
        ==
//        fvOptions(Ts)     //NOTE: MESSES UP UNITS!!!
        - fvm::Su((1-porosity)*Cf*dT, Ts)
//        (1-porosity)*Cs*(Tf-Ts)
//        (1-porosity)*Cs*rhos*cps*(Tf-Ts)
    );

    TsEqn.relax();
    fvOptions.constrain(TsEqn);
    TsEqn.solve();
    fvOptions.correct(Ts);
}
