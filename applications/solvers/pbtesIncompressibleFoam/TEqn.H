{
    S = h*A*(Tf-Tb)

    fvScalarMatrix TfEqn
    (
        fvm::ddt(porosity*rho_b*cp_b,Tf)
      // - fvm::laplacian(alphaBed, TBed)
    //  ==
    //     rho*(U&g)
      // + radiation->Sh(thermo, he)
      // + fvOptions(rho, he)
    );

    fvScalarMatrix TfEqn
    (
        fvm::ddt(porosity*rho_b*cp_b,Tf)
      // - fvm::laplacian(alphaBed, TBed)
    //  ==
    //     rho*(U&g)
      // + radiation->Sh(thermo, he)
      // + fvOptions(rho, he)
    );

    TfEqn.relax();

    fvOptions.constrain(TfEqn);

    TfEqn.solve();

    fvOptions.correct(Tf);

    // thermo.correct();
    // radiation->correct();
}