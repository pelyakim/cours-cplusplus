
#include "Cafetiere.hpp"
#include "ReservoirPoudre.hpp"







class CafetierePoudre: public Cafetiere {
    public:
        void faireLeCafe(doses_t eau, doses_t cafe) override;
        doses_t remplirCafe(doses_t) override;

    protected:
        pourcent_t getEtatCafe() const override { return cafe.getEtat(); };

    private:
        ReservoirPoudre cafe;
        void passerCafe(string&) const;
};

