{
    fvScalarMatrix TBedEqn
    (
        fvm::ddt(phib*rho_b*cp_b*TBed)
        ==
        S
      // - fvm::laplacian(alphaBed, TBed)
    //  ==
    //     rho*(U&g)
      // + radiation->Sh(thermo, he)
      // + fvOptions(rho, he)
    );

    TBedEqn.relax();

    fvOptions.constrain(TBedEqn);

    TBedEqn.solve();

    fvOptions.correct(TBed);

    // thermo.correct();
    // radiation->correct();
}
