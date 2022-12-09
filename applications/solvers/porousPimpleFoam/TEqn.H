    // Fluid temperature equation
    Info << "Solving Tf equation" << endl;
    volScalarField dT(Ts-Tf);

    fvScalarMatrix TfEqn
    (
        fvm::ddt(porosity*rhof*cpf,Tf)
        + fvm::div(rhof*cpf*phi, Tf)
        - fvm::laplacian(kf, Tf)
        ==
        fvm::Su(h*(porosity*3/rp)*dT, Tf)
        //fvm::Su(rp, Tf)
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
        - fvm::laplacian(ks, Ts)
        ==
        - fvm::Su(h*(porosity*3/rp)*dT, Ts)
        // (1-porosity)*Cs*(Tf-Ts)
        // (1-porosity)*Cs*rhos*cps*(Tf-Ts)
    );

    TsEqn.relax();
    fvOptions.constrain(TsEqn);
//    TsEqn.solve();
    fvOptions.correct(Ts);
