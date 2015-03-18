function deadboltPasswordGeneratorViewModel() {
    var self = this;

    self.encodeEngines = deadboltPasswordGenerator.getAvailableEngines();
    self.selectedEngineId = ko.observable(0);
    self.minimumPhraseLength = 6;
    self.showPassPhrase = ko.observable(false);
    self.includeSymbols = ko.observable(true);
    self.caseSensitive = ko.observable(true);
    self.characters = ko.observable(15);
    self.usePinNumber = ko.observable(false);
    self.encodedPassword = ko.observable('');
    self.pin1 = ko.observable(0);
    self.pin2 = ko.observable(0);
    self.pin3 = ko.observable(0);
    self.pin4 = ko.observable(0);
    self.memorablePhrase = ko.observable('');

    self.generatePassword = function () {
        self.encodedPassword(deadboltPasswordGenerator.encodePassword(
            self.memorablePhrase(),
            {
                engineId: self.selectedEngineId(),
                pin: self.pinNumber(),
                useSpecial: self.includeSymbols(),
                caseSensitive: self.caseSensitive(),
                passwordLength: self.characters()
            })
        );
    };

    self.minPhraseLengthMet = ko.computed(function () {
        return self.memorablePhrase().length >= 6;
    });

    self.pinNumber = ko.computed(function () {
        return '' + self.pin1() + self.pin2() + self.pin3() + self.pin4();
    });

    self.remainingChars = ko.computed(function () {
        switch (self.memorablePhrase().length) {
            case 0:
                return 'Enter at least ' + self.minimumPhraseLength + ' characters';
            case self.minimumPhraseLength - 1:
                return 'Enter at least 1 more character';
            default:
                return 'Enter at least ' + (self.minimumPhraseLength - self.memorablePhrase().length) + ' more characters';
        }
    });

    self.clearPassword = function () {
        self.memorablePhrase('');
        self.encodedPassword('');
    };

    self.passwordGenerated = ko.computed(function () {
        return self.encodedPassword().length > 0;
    });

    self.getPasswordCharacter = function (i) {
        if (this.encodedPassword().length < i) {
            return '';
        }
        return this.encodedPassword().substr(i - 1, 1);
    };

    self.raiseRevealEvent = function (type) {
        var pinStatus = this.pinNumber().length == 4 ? 'PIN enabled' : 'PIN disabled';
        switch (type) {
            case "full":
                if (!this.fullPasswordNotified) {
                    _gaq.push(['_trackEvent', 'Password Revealed', 'Full', pinStatus]);
                    this.fullPasswordNotified = true;
                }
            case "char":
                if (!this.charsNotified) {
                    _gaq.push(['_trackEvent', 'Password Revealed', 'Characters', pinStatus]);
                    this.charsNotified = true;
                }
        }
    };
}

ko.bindingHandlers.fadeVisible = {
    init: function (element, valueAccessor) {
        // Initially set the element to be instantly visible/hidden depending on the value
        var value = valueAccessor();
        $(element).toggle(ko.utils.unwrapObservable(value)); // Use "unwrapObservable" so we can handle values that may or may not be observable
    },
    update: function (element, valueAccessor) {
        // Whenever the value subsequently changes, slowly fade the element in or out
        var value = valueAccessor();
        ko.utils.unwrapObservable(value) ? $(element).slideDown() : $(element).slideUp();
    }
};


var viewModel = new deadboltPasswordGeneratorViewModel();
ko.applyBindings(viewModel);