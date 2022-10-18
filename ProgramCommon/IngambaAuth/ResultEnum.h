#pragma once

enum ResultEnum
{
	Ok = 0,
	InternalServerError = 1,
	BadPaidServiceId = 2,
	PaidServiceSupportSuspended = 3,
	BadUserLogin = 4,
	BadUserPassword = 5,
	UnsupportedPasswordEncryption = 6,
	PasswordDoesNotMatchEncryption = 7,
	UserBlocked = 8,
	BadUserId = 9,
	StatusUsingAlreadyStarted = 10,
	StatusUsingNotStartedYet = 11,
	StatusAbsent = 12,
	StatusNotProvidedThisTime = 13,
	BadAccountId = 14,
	AccountDoesNotBelongToUser = 15,
	BadAccountTypeId = 16,
	BadAssetShortcutInnerId = 17,
	CountOutOfRange = 18,
	AccountsDoNotCorrespond = 19,
	BadAssetConversionShortcutInnerId = 20,
	UserAlreadyOwnsSuchSingularAsset = 21,
	TransactionWouldMakeAccountNegative = 22,
	AssetNotAccessibleForTransactionNow = 23,
	BadPaymentChannelId = 24,
	BadTransactionId = 25,
	InputTransactionIsAReversedOne = 26,

};

