#ifndef PF_FLAG_H
#define PF_FLAG_H


/**
 * Base class for objects which have flags.
 *
 * @author Andrew Mustun
 */
struct PF_Flag {
    //! \{Constructor with initialisation to the given flags.
    //! Default sets all flags to 0
    PF_Flag(unsigned f = 0);
    //! \}

    virtual ~PF_Flag() = default;

    unsigned getFlags() const;

    void resetFlags();

    void setFlags(unsigned f);

    void setFlag(unsigned f);

    void delFlag(unsigned f);

    void toggleFlag(unsigned f);

    bool getFlag(unsigned f) const;

private:
    unsigned flags = 0;
};

#endif // PF_FLAG_H
