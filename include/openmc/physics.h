#ifndef OPENMC_PHYSICS_H
#define OPENMC_PHYSICS_H

#include "openmc/bank.h"
#include "openmc/nuclide.h"
#include "openmc/particle.h"
#include "openmc/position.h"
#include "openmc/reaction.h"

namespace openmc {

//==============================================================================
// Non-member functions
//==============================================================================

//! Sample a nuclide and reaction and then calls the appropriate routine
extern "C" void collision(Particle* p);

//! Samples an incident neutron reaction
void sample_neutron_reaction(Particle* p);

//! Samples an element based on the macroscopic cross sections for each nuclide
//! within a material and then samples a reaction for that element and calls the
//! appropriate routine to process the physics.
extern "C" void sample_photon_reaction(Particle* p);

//! Terminates the particle and either deposits all energy locally
//! (electron_treatment = ELECTRON_LED) or creates secondary bremsstrahlung
//! photons from electron deflections with charged particles (electron_treatment
//! = ELECTRON_TTB).
void sample_electron_reaction(Particle* p);

//! Terminates the particle and either deposits all energy locally
//! (electron_treatment = ELECTRON_LED) or creates secondary bremsstrahlung
//! photons from electron deflections with charged particles (electron_treatment
//! = ELECTRON_TTB). Two annihilation photons of energy MASS_ELECTRON_EV (0.511
//! MeV) are created and travel in opposite directions.
void sample_positron_reaction(Particle* p);

void sample_nuclide(const Particle* p, int mt, int* i_nuclide, int* i_nuc_mat);

//! Determine the average total, prompt, and delayed neutrons produced from
//! fission and creates appropriate bank sites.
void create_fission_sites(Particle* p, int i_nuclide, const Reaction* rx,
  Bank* bank_array, int64_t* bank_size, int64_t bank_capacity);

// void sample_element(Particle* p);

Reaction* sample_fission(int i_nuclide, double E);

void sample_photon_product(int i_nuclide, double E, int* i_rx, int* i_product);

void absorption(Particle* p, int i_nuclide);

void scatter(Particle*, int i_nuclide, int i_nuc_mat);

//! Treats the elastic scattering of a neutron with a target.
void elastic_scatter(int i_nuclide, const Reaction* rx, double kT, double* E,
  double* uvw, double* mu_lab, double* wgt);

extern "C" void sab_scatter(int i_nuclide, int i_sab, double* E,
  double* uvw, double* mu);

//! samples the target velocity. The constant cross section free gas model is
//! the default method. Methods for correctly accounting for the energy
//! dependence of cross sections in treating resonance elastic scattering such
//! as the DBRC, WCM, and a new, accelerated scheme are also implemented here.
Direction sample_target_velocity(const Nuclide* nuc, double E, Direction u,
   Direction v_neut, double xs_eff, double kT, double* wgt);

//! samples a target velocity based on the free gas scattering formulation, used
//! by most Monte Carlo codes, in which cross section is assumed to be constant
//! in energy. Excellent documentation for this method can be found in
//! FRA-TM-123.
Direction sample_cxs_target_velocity(double awr, double E, Direction u, double kT);

void sample_fission_neutron(int i_nuclide, const Reaction* rx, double E_in, Bank* site);

//! handles all reactions with a single secondary neutron (other than fission),
//! i.e. level scattering, (n,np), (n,na), etc.
void inelastic_scatter(const Nuclide* nuc, const Reaction* rx, Particle* p);

void sample_secondary_photons(Particle* p, int i_nuclide);

extern "C" void thick_target_bremsstrahlung(Particle* p, double* E_lost);

} // namespace openmc

#endif // OPENMC_PHYSICS_H
